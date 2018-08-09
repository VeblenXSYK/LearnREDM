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

		CStringW imgPath;
		imgPath.Format(L"%s\\%s", CCommModule::GetPicRootDir(), L"\\Person\\test.jpg");
		Gdiplus::Image imgData(imgPath);
		graphics.DrawImage(&imgData, m_rcWindow.left, m_rcWindow.top, m_rcWindow.Width(), m_rcWindow.Height());
	}

	//重置绘图的所有变换
	graphics.ResetTransform();
	graphics.Save();
}

void PersonPreview::OnLButtonDown(UINT nFlags, CPoint pt)
{
	// 判断按下鼠标的模式
	CRect rcItem;
	DV_GetClientRect(&rcItem);
	
	if (pt.x < rcItem.left + 20 || (pt.x < rcItem.right && pt.x > rcItem.right-20)
		|| pt.y < rcItem.top + 20 || (pt.y < rcItem.bottom && pt.y > rcItem.bottom - 20))
	{
		m_iMode = ZoomMode;
	}
	else
	{
		m_iMode = MoveMode;
	}

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
	CRect rcItem;
	DV_GetClientRect(&rcItem);
	if (pt.x < rcItem.left + 20)									// 左边沿
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		if (m_bDown)
		{
			OnDragLeft(pt);
		}
	}
	else if (pt.x < rcItem.right && pt.x > rcItem.right - 20)		//右边沿
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZEWE));
		if (m_bDown)
		{
			OnDragRight(pt);
		}
	}
	else if (pt.y < rcItem.top + 20)								// 上边沿
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		if (m_bDown)
		{
			OnDragTop(pt);
		}
	}
	else if (pt.y < rcItem.bottom && pt.y > rcItem.bottom - 20)		//下边沿
	{
		::SetCursor(::LoadCursor(NULL, IDC_SIZENS));
		if (m_bDown)
		{
			OnDragBottom(pt);
		}
	}
	else															// 移动
	{
		if (m_bDown)
		{
			m_TrackDragPt = pt;
			CPoint ptOffset = m_TrackDragPt - m_StartDragPt;
			CRect rcWnd = m_StartDragRc;
			rcWnd.OffsetRect(ptOffset);
			DM_FloatLayout(rcWnd);
			p_Parent->DM_Invalidate();
			SetAttribute(L"cursor", L"ds_tool_move");
		}
	}
}

void PersonPreview::OnDragLeft(CPoint pt)
{
	m_TrackDragPt = pt;
	CPoint ptOffset = m_TrackDragPt - m_StartDragPt;
	CRect rcWnd = m_StartDragRc;
	rcWnd.left = rcWnd.left + ptOffset.x;
	DM_FloatLayout(rcWnd);
	p_Parent->DM_Invalidate();

	// LOG_USER("%d,%d,%d,%d\n", rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);
}

void PersonPreview::OnDragRight(CPoint pt)
{
	m_TrackDragPt = pt;
	CPoint ptOffset = m_TrackDragPt - m_StartDragPt;
	CRect rcWnd = m_StartDragRc;
	rcWnd.right = rcWnd.right + ptOffset.x;
	DM_FloatLayout(rcWnd);
	p_Parent->DM_Invalidate();
}

void PersonPreview::OnDragTop(CPoint pt)
{
	m_TrackDragPt = pt;
	CPoint ptOffset = m_TrackDragPt - m_StartDragPt;
	CRect rcWnd = m_StartDragRc;
	rcWnd.top = rcWnd.top + ptOffset.y;
	DM_FloatLayout(rcWnd);
	p_Parent->DM_Invalidate();
}

void PersonPreview::OnDragBottom(CPoint pt)
{
	m_TrackDragPt = pt;
	CPoint ptOffset = m_TrackDragPt - m_StartDragPt;
	CRect rcWnd = m_StartDragRc;
	rcWnd.bottom = rcWnd.bottom + ptOffset.y;
	DM_FloatLayout(rcWnd);
	p_Parent->DM_Invalidate();
}

void PersonPreview::OnDragLeftTop(CPoint pt)
{

}

void PersonPreview::OnDragRightTop(CPoint pt)
{

}

void PersonPreview::OnDragRightBottom(CPoint pt)
{

}

void PersonPreview::OnDragLeftBottom(CPoint pt)
{

}

