#include "StdAfx.h"
#include "PersonPreview.h"
#include "CommModule.h"

#pragma comment(lib, "Gdiplus.lib")

const double PI = 3.1415926535898;

CPersonPreview::CPersonPreview()
{
	m_bDown = false;
	m_iMode = NoneMode;
	m_AngleOfRotation = 0;
	m_pImgData = NULL;

	//初始化gdi+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

CPersonPreview::~CPersonPreview()
{
	//释放gdi+
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

//void PersonPreview::Rotate(HDC hDC, float angle)
//{
//	Gdiplus::Graphics graphics(hDC);
//
//	IDMBitmap* ppBitmap;
//	m_pSkin->GetBitmap(&ppBitmap);
//	Gdiplus::Bitmap bmp(ppBitmap->GetBitmap(), NULL);
//	Gdiplus::Image *image = (Gdiplus::Image *)&bmp;
//
//	// 获取图像的中心点
//	int centerPosX = m_rcWindow.left + m_rcWindow.Width() / 2;
//	int centerPosY = m_rcWindow.top + m_rcWindow.Height() / 2;
//	int width = m_rcWindow.Width();
//	int height = m_rcWindow.Height();
//
//	// 计算左上角绕矩形中心顺时针旋转后的坐标
//	int x = (m_rcWindow.left - centerPosX)*cos(angle*PI/180) - (m_rcWindow.top - centerPosY)*sin(angle*PI/180) + centerPosX;
//	int y = (m_rcWindow.left - centerPosX)*sin(angle*PI/180) + (m_rcWindow.top - centerPosY)*cos(angle*PI/180) + centerPosY;
//
//	// 原点移动到矩形中心点
//	graphics.TranslateTransform(centerPosX, centerPosY);
//	graphics.DrawImage(image, 0, 0, m_rcWindow.Width(), m_rcWindow.Height());
//
//	m_rcWindow = { centerPosX, centerPosY, centerPosX+width, centerPosY+height};
//	//// 旋转（沿当前原点）
//	//graphics.RotateTransform(angle/*, MatrixOrderAppend*/);	
//	//graphics.DrawImage(image, 0, 0, m_rcWindow.Width(), m_rcWindow.Height());
//	//// 还原原点
//	//graphics.TranslateTransform(-centerPosX, -centerPosY);
//	//graphics.DrawImage(image, 0, 0, m_rcWindow.Width(), m_rcWindow.Height());
//
//	//重置绘图的所有变换
//	graphics.ResetTransform();
//	graphics.Save();
//}

void CPersonPreview::Rotate(Gdiplus::Graphics &graphics, float angle)
{
	// 获取矩形中心点
	Gdiplus::REAL centerPosX = static_cast<Gdiplus::REAL>(m_rcWindow.left + m_rcWindow.Width() / 2);
	Gdiplus::REAL centerPosY = static_cast<Gdiplus::REAL>(m_rcWindow.top + m_rcWindow.Height() / 2);
	// 原点移动到矩形中心点
	graphics.TranslateTransform(centerPosX, centerPosY);
	// 旋转（沿当前原点）
	graphics.RotateTransform(angle/*, MatrixOrderAppend*/);
	// 还原原点
	graphics.TranslateTransform(-centerPosX, -centerPosY);
}

void CPersonPreview::ModifyAngle(void)
{
	m_AngleOfRotation += 45;
	if (m_AngleOfRotation == 360)
		m_AngleOfRotation = 0;
	p_Parent->DM_Invalidate();
}

void CPersonPreview::LoadImage(wchar_t *imgpath)
{
	m_picPath = imgpath;
	// 加载图片
	m_pImgData.reset(new Gdiplus::Image(imgpath));
}

void CPersonPreview::LoadPsImageData()
{
	// 获取当前激活图片截图
	std::string outMsg;
	PSGetDocumentImage(CCommModule::GetPSHandle(), 288, 493, outMsg);

	// 保存图片数据
	m_sFromPsData = std::move(outMsg);
}

Gdiplus::Image *CPersonPreview::CreateBitmapFromMemory(LPCVOID pData, SIZE_T sizeImage)
{
	//HGLOBAL hImageData = ::GlobalAlloc(GMEM_MOVEABLE, sizeImage);
	//if (hImageData == NULL) 
	//	return NULL;

	//LPVOID pImgData = ::GlobalLock(hImageData);
	//if (pImgData == NULL)
	//{
	//	::GlobalFree(hImageData);
	//	return NULL;
	//}

	//::memcpy(pImgData, pData, sizeImage);
	//::GlobalUnlock(hImageData);

	//Gdiplus::Image *pImg = NULL;
	//IStream *ps = NULL;
	//if (::CreateStreamOnHGlobal(hImageData, TRUE, &ps) == S_OK)
	//{
	//	pImg = Gdiplus::Image::FromStream(ps);
	//	// _SanUiGdiImageValid(pImg);
	//}

	//ps->Release();
	//::GlobalFree(hImageData);

	//return pImg;

	//图片分配全局存储空间
	HGLOBAL hGlobalImage = GlobalAlloc(GMEM_MOVEABLE, sizeImage);
	LPVOID pvDataImage = NULL;
	if (hGlobalImage == NULL)
	{
		return NULL;
	}
	//锁定分配内存块
	if ((pvDataImage = GlobalLock(hGlobalImage)) == NULL)
	{
		return NULL;
	}
	memcpy(pvDataImage, pData, sizeImage);
	GlobalUnlock(hGlobalImage);

	//参数设置为TRUE，则hGlobalImage最终会自动释放
	IStream *pStmImage = NULL;
	CreateStreamOnHGlobal(hGlobalImage, FALSE, &pStmImage);

	//if (::CreateStreamOnHGlobal(hImageData, TRUE, &ps) == S_OK)
	//{
	//	pImg = Gdiplus::Image::FromStream(ps);
	//	// _SanUiGdiImageValid(pImg);
	//}

	//ps->Release();
	//::GlobalFree(hImageData);

	Gdiplus::Image *pImg = new Gdiplus::Image(pStmImage);

	return pImg;
	// pStmImage->Release();
}

int CPersonPreview::JudgeDragType(CPoint pt)
{
	CRect rcItem;
	DV_GetClientRect(&rcItem);
	if (pt.x < rcItem.left + 20 && pt.y < rcItem.top + 20)
		return DragLeftTop;
	else if (pt.x < rcItem.left + 20 && pt.y > rcItem.bottom - 20)
		return DragLeftBottom;
	else if (pt.x > rcItem.right - 20 && pt.y < rcItem.top + 20)
		return DragRightTop;
	else if (pt.x > rcItem.right - 20 && pt.y > rcItem.bottom - 20)
		return DragRightBottom;
	else if (pt.x < rcItem.left + 20)
		return DragLeft;
	else if (pt.y < rcItem.top + 20)
		return DragTop;
	else if (pt.x > rcItem.right - 20)
		return DragRight;
	else if (pt.y > rcItem.bottom - 20)
		return DragBottom;
	else
		return MoveMode;
}

void CPersonPreview::HandleDrag(CPoint pt, int type)
{
	m_TrackDragPt = pt;
	CPoint ptOffset = m_TrackDragPt - m_StartDragPt;
	CRect rcWnd = m_StartDragRc;

	switch (type)
	{
	case DragLeft:
		rcWnd.left = rcWnd.left + ptOffset.x;
		break;

	case DragRight:
		rcWnd.right = rcWnd.right + ptOffset.x;
		break;

	case DragTop:
		rcWnd.top = rcWnd.top + ptOffset.y;
		break;

	case DragBottom:
		rcWnd.bottom = rcWnd.bottom + ptOffset.y;
		break;

	case DragLeftTop:
		rcWnd.left = rcWnd.left + ptOffset.x;
		rcWnd.top = rcWnd.top + ptOffset.y;
		break;

	case DragLeftBottom:
		rcWnd.left = rcWnd.left + ptOffset.x;
		rcWnd.bottom = rcWnd.bottom + ptOffset.y;
		break;

	case DragRightTop:
		rcWnd.right = rcWnd.right + ptOffset.x;
		rcWnd.top = rcWnd.top + ptOffset.y;
		break;

	case DragRightBottom:
		rcWnd.right = rcWnd.right + ptOffset.x;
		rcWnd.bottom = rcWnd.bottom + ptOffset.y;
		break;
	case MoveMode:
		rcWnd.OffsetRect(ptOffset);
		break;
	}

	DM_FloatLayout(rcWnd);
	p_Parent->DM_Invalidate();
}

void CPersonPreview::DM_OnPaint(IDMCanvas* pCanvas)
{
	// 获取画布HDC
	HDC hDC = pCanvas->GetDC();

	// 使用GDI+
	Gdiplus::Graphics graphics(hDC);

	if (m_pSkin)
	{
		IDMBitmap* ppBitmap;
		m_pSkin->GetBitmap(&ppBitmap);
		Gdiplus::Bitmap bmp(ppBitmap->GetBitmap(), NULL);
		Gdiplus::Image *image = (Gdiplus::Image *)&bmp;
		Rotate(graphics, 45);
		graphics.DrawImage(image, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());

		// m_pSkin->Draw(pCanvas, m_rcWindow, 0);

		//// 获取图片的尺寸
		//CSize sz;
		//m_pSkin->GetStateSize(sz);

		//// 获取控件的尺寸
		//CRect rcItem, originalItem;
		//this->DV_GetClientRect(&rcItem);
		//originalItem = rcItem;

		//// 把图片按照控件等比例缩放
		//MeetRect(rcItem, sz);
		//m_pSkin->Draw(pCanvas, rcItem, 0);

		//// 填充背景色
		//CRect rcDraw = originalItem;
		//rcDraw.DeflateRect(0, 1, 0, 1);
		//pCanvas->FillSolidRect(rcDraw, PBGRA(0, 0, 0, 0X80));
	}
	else
	{
		Rotate(graphics, m_AngleOfRotation);
		// graphics.DrawImage(m_pImgData.get(), m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());

		Gdiplus::Image *image = CreateBitmapFromMemory(m_sFromPsData.c_str(), m_sFromPsData.size());   //把你的图像数据加载入内存
		graphics.DrawImage(image, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());
	}

	//重置绘图的所有变换
	//graphics.ResetTransform();
	//graphics.Save();
}

void CPersonPreview::OnLButtonDown(UINT nFlags, CPoint pt)
{
	DM_SetCapture();
	m_bDown = true;
	m_StartDragPt = pt;
	m_TrackDragPt = m_StartDragPt;
	m_StartDragRc = m_rcWindow;
}

void CPersonPreview::OnLButtonUp(UINT nFlags, CPoint pt)
{
	m_bDown = false;
	DM_ReleaseCapture();
}

void CPersonPreview::OnMouseMove(UINT nFlags, CPoint pt)
{
	// 判断拖曳类型
	int type = JudgeDragType(pt);

	switch (type)
	{
		case DragLeft:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			break;

		case DragRight:
			::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
			break;

		case DragTop:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			break;

		case DragBottom:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
			break;

		case DragLeftTop:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			break;

		case DragLeftBottom:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			break;

		case DragRightTop:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENESW));
			break;

		case DragRightBottom:
			::SetCursor(::LoadCursor(NULL, IDC_SIZENWSE));
			break;

		case MoveMode:
			if (m_bDown) ::SetCursor(::LoadCursor(NULL, IDC_HAND));			
			break;
	}

	// 处理拖曳
	if(m_bDown)
		HandleDrag(pt, type);
}

