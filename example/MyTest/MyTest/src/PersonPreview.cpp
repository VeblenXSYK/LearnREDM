#include "StdAfx.h"
#include "PersonPreview.h"
#include "CommModule.h"

#pragma comment(lib, "Gdiplus.lib")

const double PI = 3.1415926535898;

PersonPreview::PersonPreview()
{
	m_bDown = false;
	m_iMode = NoneMode;
	m_AngleOfRotation = 0;
	m_pImgData = NULL;

	//初始化gdi+
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

PersonPreview::~PersonPreview()
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

void PersonPreview::Rotate(Gdiplus::Graphics &graphics, float angle)
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

void PersonPreview::ModifyAngle(void)
{
	m_AngleOfRotation += 45;
	if (m_AngleOfRotation == 360)
		m_AngleOfRotation = 0;
	p_Parent->DM_Invalidate();
}

void PersonPreview::LoadImage(wchar_t *imgpath)
{
	// 加载图片
	m_pImgData.reset(new Gdiplus::Image(imgpath));
}

int PersonPreview::JudgeDragType(CPoint pt)
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

void PersonPreview::HandleDrag(CPoint pt, int type)
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

void PersonPreview::DM_OnPaint(IDMCanvas* pCanvas)
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
		graphics.DrawImage(m_pImgData.get(), m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());
	}

	//重置绘图的所有变换
	//graphics.ResetTransform();
	//graphics.Save();
}

void PersonPreview::OnLButtonDown(UINT nFlags, CPoint pt)
{
	DM_SetCapture();
	m_bDown = true;
	m_StartDragPt = pt;
	m_TrackDragPt = m_StartDragPt;
	m_StartDragRc = m_rcWindow;
}

void PersonPreview::OnLButtonUp(UINT nFlags, CPoint pt)
{
	m_bDown = false;
	DM_ReleaseCapture();
}

void PersonPreview::OnMouseMove(UINT nFlags, CPoint pt)
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

