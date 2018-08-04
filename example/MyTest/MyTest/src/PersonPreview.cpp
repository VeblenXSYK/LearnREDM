#include "StdAfx.h"
#include "PersonPreview.h"


PersonPreview::PersonPreview()
{
	m_bDown = false;
	m_iMode = NoneMode;
}

void PersonPreview::DM_OnPaint(IDMCanvas* pCanvas)
{
	if (m_pSkin)
	{
		// 获取图片的尺寸
		CSize sz;
		m_pSkin->GetStateSize(sz);

		// 获取控件的尺寸
		CRect rcItem, originalItem;
		this->DV_GetClientRect(&rcItem);
		originalItem = rcItem;

		// 把图片按照控件等比例缩放
		MeetRect(rcItem, sz);

		m_pSkin->Draw(pCanvas, rcItem, 0);

		CRect rcDraw = originalItem;
		rcDraw.DeflateRect(0, 1, 0, 1);

		pCanvas->FillSolidRect(rcDraw, PBGRA(0, 0, 0, 0X80));
	}
	else
	{
		DMColor ClrBg;
		m_pDUIXmlInfo->m_pStyle->GetBgColor(0, ClrBg);
		if (!ClrBg.IsTextInvalid())
		{
			pCanvas->FillSolidRect(&m_rcWindow, ClrBg);
		}
	}
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

