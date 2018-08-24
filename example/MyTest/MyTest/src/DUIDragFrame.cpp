#include "StdAfx.h"
#include "DUIDragFrame.h"
#include "PersonPreview.h"
#include "MainWnd.h"

extern CMainWnd* g_pMainWnd;

DMCode AutoDrawRoundRect(IDMCanvas*pCanvas, DMColor TextClr, int iStyle, int iWidth, LPCRECT lpRect, POINT &pt)
{
	DMCode iErr = DM_ECODE_FAIL;
	do
	{
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->CreateRegObj((void**)&pRender, NULL, DMREG_Render);
		DMSmartPtrT<IDMPen> pPen;
		pRender->CreatePen(TextClr, iStyle, iWidth, &pPen);

		DMSmartPtrT<IDMPen> pOldPen;
		pCanvas->SelectObject(pPen, (IDMMetaFile**)&pOldPen);;

		pCanvas->DrawRoundRect(lpRect, pt);

		pCanvas->SelectObject(pOldPen);
		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DragMeta::DragMeta(int idx/*=-1*/,HCURSOR hCursor /*= ::LoadCursor(NULL,IDC_ARROW)*/)
{
	m_Index		 = idx;
	m_hCursor	 = hCursor;
	m_Clr		 = PBGRA(0,0,0xff,0xff);
	m_GrayClr	 = PBGRA(198,198,198,0xff);
	m_bDragDot	 = true;
	m_bEnable	 = true;     
}

DMCode DragMeta::OnRender(IDMCanvas* pCanvas)
{
	if (0 == m_Index)
	{
		AutoDrawRoundRect(pCanvas,m_Clr,PS_SOLID,1,m_Rect,CPoint(0,0));
	}
	else if (m_Index<9)
	{
		AutoDrawRoundRect(pCanvas,m_bEnable?m_Clr:m_GrayClr,PS_SOLID,1,m_Rect,CPoint(0,0));
	}
	else 
	{
		if (m_pSkin)
		{
			m_pSkin->Draw(pCanvas,m_Rect,m_bEnable?0:1);
		}
	}

	return DM_ECODE_OK;
}

DMCode DragMeta::SetCurSor()
{
	if (m_bEnable)
	{
		::SetCursor(m_hCursor);
	}
	return DM_ECODE_OK;
}

// DUIDragFrame--------------------------------------
#define  DRAGSIZE    6
DUIDragFrame::DUIDragFrame()
{
	m_pCurdragMeta = NULL;
	m_dragMetaCount = 10;
	m_pData = NULL;
	m_bDown = false;
	m_pDUIXmlInfo->m_bFocusable = true;

	LinkDragMetaEvent();
}
 
DMCode DUIDragFrame::InitDragFrame(DUIWindow* pWnd,CRect& rcLayout)
{
	DMCode iErr = DM_ECODE_FAIL;
	do 
	{
		CRect rcDrag = pWnd->m_rcWindow;
		if (rcDrag.IsRectEmpty())
		{
			break;
		}

		// LOG_USER("%d,%d,%d,%d\n", rcDrag.left, rcDrag.top, rcDrag.right, rcDrag.bottom);
		InitDragMeta(rcDrag, false, false);

		// 计算自身frame的区域
		rcLayout = CalcDragFrameRect(rcDrag);
		// LOG_USER("%d,%d,%d,%d\n", rcLayout.left, rcLayout.top, rcLayout.right, rcLayout.bottom);
		m_pData = pWnd;

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

// private
void DUIDragFrame::LinkDragMetaEvent()
{
	m_dragMetas[0] = DragMeta(0, ::LoadCursor(NULL,IDC_HAND));
	m_dragMetas[0].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragMove,this);

	m_dragMetas[1] = DragMeta(1, ::LoadCursor(NULL,IDC_SIZEWE));
	m_dragMetas[1].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragLeft,this);

	m_dragMetas[2] = DragMeta(2, ::LoadCursor(NULL,IDC_SIZENWSE));
	m_dragMetas[2].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragLeftTop,this);

	m_dragMetas[3] = DragMeta(3, ::LoadCursor(NULL,IDC_SIZENS));
	m_dragMetas[3].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragTop,this);

	m_dragMetas[4] = DragMeta(4, ::LoadCursor(NULL,IDC_SIZENESW));
	m_dragMetas[4].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragRightTop,this);

	m_dragMetas[5] = DragMeta(5, ::LoadCursor(NULL,IDC_SIZEWE));
	m_dragMetas[5].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragRight,this);

	m_dragMetas[6] = DragMeta(6, ::LoadCursor(NULL,IDC_SIZENWSE));
	m_dragMetas[6].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragRightBottom,this);

	m_dragMetas[7] = DragMeta(7, ::LoadCursor(NULL,IDC_SIZENS));
	m_dragMetas[7].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragBottom,this);

	m_dragMetas[8] = DragMeta(8, ::LoadCursor(NULL,IDC_SIZENESW));
	m_dragMetas[8].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragLeftBottom,this);

	m_dragMetas[9] = DragMeta(9, ::LoadCursor(NULL,IDC_HAND));
	m_dragMetas[9].m_DragMetaAction = DMBind(&DUIDragFrame::OnDragBox,this);
}

void DUIDragFrame::InitDragMeta(CRect Rect,bool bMain,bool bAllGray)
{
	m_dragMetas[0].m_Rect = Rect;
	Rect.InflateRect(DRAGSIZE, DRAGSIZE);

	m_dragMetas[1].m_Rect = Rect;
	m_dragMetas[1].m_Rect.top += (Rect.Height() - DRAGSIZE) / 2;
	m_dragMetas[1].m_Rect.bottom = m_dragMetas[1].m_Rect.top + DRAGSIZE;
	m_dragMetas[1].m_Rect.right = m_dragMetas[1].m_Rect.left + DRAGSIZE;

	CRect rrr = m_dragMetas[1].m_Rect;

	m_dragMetas[2].m_Rect = Rect;
	m_dragMetas[2].m_Rect.bottom = Rect.top + DRAGSIZE;
	m_dragMetas[2].m_Rect.right = Rect.left + DRAGSIZE;

	rrr = m_dragMetas[2].m_Rect;

	m_dragMetas[3].m_Rect = Rect;
	m_dragMetas[3].m_Rect.left += (Rect.Width() - DRAGSIZE) / 2;
	m_dragMetas[3].m_Rect.right = m_dragMetas[3].m_Rect.left + DRAGSIZE;
	m_dragMetas[3].m_Rect.bottom = Rect.top + DRAGSIZE;

	m_dragMetas[4].m_Rect = Rect;
	m_dragMetas[4].m_Rect.left = Rect.right - DRAGSIZE;
	m_dragMetas[4].m_Rect.bottom = m_dragMetas[4].m_Rect.top + DRAGSIZE;

	// Right
	m_dragMetas[5].m_Rect = Rect;
	m_dragMetas[5].m_Rect.top += (Rect.Height() - DRAGSIZE) / 2;
	m_dragMetas[5].m_Rect.bottom = m_dragMetas[5].m_Rect.top + DRAGSIZE;
	m_dragMetas[5].m_Rect.left = Rect.right - DRAGSIZE;

	// RightBottom
	m_dragMetas[6].m_Rect = Rect;
	m_dragMetas[6].m_Rect.top = Rect.bottom - DRAGSIZE;
	m_dragMetas[6].m_Rect.left = Rect.right - DRAGSIZE;

	// Bottom
	m_dragMetas[7].m_Rect = Rect;
	m_dragMetas[7].m_Rect.left += (Rect.Width() - DRAGSIZE) / 2;
	m_dragMetas[7].m_Rect.right = m_dragMetas[7].m_Rect.left + DRAGSIZE;
	m_dragMetas[7].m_Rect.top = Rect.bottom - DRAGSIZE;

	// LeftBottom
	m_dragMetas[8].m_Rect = Rect;
	m_dragMetas[8].m_Rect.right = Rect.left + DRAGSIZE;
	m_dragMetas[8].m_Rect.top = Rect.bottom - DRAGSIZE;

	// DragMode
	m_dragMetas[9].m_Rect.right = m_dragMetas[2].m_Rect.left;
	m_dragMetas[9].m_Rect.bottom = m_dragMetas[2].m_Rect.top;
	m_dragMetas[9].m_Rect.left = m_dragMetas[2].m_Rect.left - 24;
	m_dragMetas[9].m_Rect.top = m_dragMetas[2].m_Rect.top - 24;
	m_dragMetas[9].m_Clr = PBGRA(0,0,0XFF,0XFF);
	m_dragMetas[9].m_pSkin = g_pDMApp->GetSkin(L"ds_drag_move");

	for (int i=0;i<m_dragMetaCount;i++)
	{
		m_dragMetas[i].m_bEnable = true;
	}

	m_dragMetas[1].m_bEnable = !bMain;
	m_dragMetas[2].m_bEnable = !bMain;
	m_dragMetas[3].m_bEnable = !bMain;
	m_dragMetas[4].m_bEnable = !bMain;
	m_dragMetas[8].m_bEnable = !bMain;
	m_dragMetas[9].m_bEnable = !bMain;

	if (bAllGray)
	{
		for (int i=0;i<m_dragMetaCount;i++)
		{
			m_dragMetas[i].m_bEnable = false;
		}
	}
}

DragMeta* DUIDragFrame::HittestDragMeta(CPoint pt,bool bMain)
{
	int iCount = m_dragMetaCount;

	for (int i = 1; i < iCount; ++i)
	{
		if (m_dragMetas[i].m_Rect.PtInRect(pt))
		{
			return &m_dragMetas[i];
		}
	}

	if (m_dragMetas[0].m_Rect.PtInRect(pt))
	{
		return &m_dragMetas[0];
	}

	return NULL;
}

void DUIDragFrame::OnDragLeft(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.left, NULL);
	}
}

void DUIDragFrame::OnDragLeftTop(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.left, (int*)&rect.top);
	}
}

void DUIDragFrame::OnDragTop(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, NULL, (int*)&rect.top);
	}
}

void DUIDragFrame::OnDragRightTop(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.right, (int*)&rect.top);
	}
}

void DUIDragFrame::OnDragRight(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.right, NULL);
	}
}

void DUIDragFrame::OnDragRightBottom(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.right, (int*)&rect.bottom);
	}
}

void DUIDragFrame::OnDragBottom(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, NULL, (int*)&rect.bottom);
	}
}

void DUIDragFrame::OnDragLeftBottom(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect;
		InternalHandleDrag(rect, (int*)&rect.left, (int*)&rect.bottom);
	}
}

void DUIDragFrame::OnDragBox(DragMeta& meta, int iAction)
{
	if (meta.m_bEnable)
	{
		CRect rect = m_StartDragRc; // rect总是表示当前大小
		int x = m_TrackDragPt.x - m_StartDragPt.x;
		int y = m_TrackDragPt.y - m_StartDragPt.y;
		rect.OffsetRect(x,y);

		// 获取父窗口位置
		CRect prect;
		p_Parent->DV_GetClientRect(&prect);
		// 计算出相对于父窗口的偏移矩形
		rect.OffsetRect(-prect.left, -prect.top);

		CStringW strClrbg;
		strClrbg.Format(L"%d,%d,%d,%d", rect.left, rect.top, rect.right, rect.bottom);
		m_pData->SetAttribute(L"pos", strClrbg, false);

		CRect rcFrame;
		InitDragFrame(m_pData, rcFrame);
		DM_SetVisible(true, true);
		DM_Invalidate();
	}
}

void DUIDragFrame::OnDragMove(DragMeta& meta, int iAction)
{
	OnDragBox(meta,iAction);// 暂时屏蔽
}

CRect DUIDragFrame::CalcDragFrameRect(CRect rcDrag)
{
	CRect rcLayout;
	rcLayout = rcDrag;
	rcLayout.InflateRect(DRAGSIZE,DRAGSIZE);
	rcLayout.left -= 24;//24为box图标的大小
	rcLayout.top  -= 24;
	return rcLayout;
}

// public
void DUIDragFrame::DM_OnPaint(IDMCanvas* pCanvas)
{
	int i = 1;
	if (m_bDown&&NULL != m_pCurdragMeta&&0 == m_pCurdragMeta->m_Index)
	{
		i = 0;
	}
	for (;i<=9;i++)
	{
		m_dragMetas[i].OnRender(pCanvas);
	}
}

void DUIDragFrame::OnLButtonDown(UINT nFlags,CPoint pt)
{
	DragMeta* pMeta = HittestDragMeta(pt,false);
	if (pMeta)
	{
		DV_SetFocusWnd();
		DM_SetCapture();
		m_bDown = true;
		pMeta->SetCurSor();
		m_StartDragPt = pt;
		m_TrackDragPt = m_StartDragPt;
		m_StartDragRc = m_dragMetas[0].m_Rect;
		// LOG_USER("m_StartDragRc: %d,%d,%d,%d\n", m_StartDragRc.left, m_StartDragRc.top, m_StartDragRc.right, m_StartDragRc.bottom);
		// LOG_USER("m_StartDragRt: %d,%d\n", m_StartDragPt.x, m_StartDragPt.y);
	}
}

void DUIDragFrame::OnLButtonUp(UINT nFlags,CPoint pt)
{
	m_bDown = false;
	DM_ReleaseCapture();
	DM_Invalidate();
}

void DUIDragFrame::OnRButtonDown(UINT nFlags, CPoint pt)
{
}

void DUIDragFrame::OnMouseMove(UINT nFlags,CPoint pt)
{
	if (!m_bDown)
	{
		m_pCurdragMeta = HittestDragMeta(pt,false);
		if (m_pCurdragMeta)
		{
			m_pCurdragMeta->SetCurSor();
		}	
	}
	else if (m_bDown&&m_pCurdragMeta)
	{
		m_pCurdragMeta->SetCurSor();
		m_TrackDragPt = pt;
		// LOG_USER("m_TrackDragPt: %d,%d\n", m_TrackDragPt.x, m_TrackDragPt.y);
		if (m_TrackDragPt != m_StartDragPt)
		{
			m_pCurdragMeta->m_DragMetaAction(*m_pCurdragMeta,0);
		}
	}
}

DUIWND DUIDragFrame::HitTestPoint(CPoint pt,bool bFindNoMsg)
{
	//if (!m_bDown)
	//{// 拖动过程时,保持m_pCurdragMeta不变
	//	m_pCurdragMeta = HittestDragMeta(pt,false);
	//}

	//if (NULL != m_pCurdragMeta && 0 == m_pCurdragMeta->m_Index)
	//{
	//	m_pData->HitTestPoint(pt, true);
	//}
	//else if (NULL == m_pCurdragMeta)
	//{
	//	return m_pData->HitTestPoint(pt, true);// 注意，这里使用了return,因为它不在dragframe的范围内了
	//}
	//if (m_pData&&NULL != m_pCurdragMeta && 0 != m_pCurdragMeta->m_Index)
	//{// 在DUIObjEditor保证了m_pDragFrame->DM_SetWndToTop(),从而HitTestPoint从dragframe->designeditor
	//	g_pMainWnd->HoverInSelOrAddMode(m_pData);
	//}
	
	return m_hDUIWnd;
}

void DUIDragFrame::InternalHandleDrag(CRect& rect, int* pHori, int* pVert)
{
	do
	{
		int x = (pHori ? (m_TrackDragPt.x - m_StartDragPt.x) : 0);
		int y = (pVert ? (m_TrackDragPt.y - m_StartDragPt.y) : 0);

		rect = m_StartDragRc; // rect总是表示当前大小
		if (pHori)
		{
			*pHori += x;
		}
		if (pVert)
		{
			*pVert += y;
		}

		rect.NormalizeRect();

		// 获取父窗口位置
		CRect prect;
		p_Parent->DV_GetClientRect(&prect);
		// 计算出相对于父窗口的偏移矩形
		rect.OffsetRect(-prect.left, -prect.top);

		CStringW strClrbg;
		strClrbg.Format(L"%d,%d,%d,%d", rect.left, rect.top, rect.right, rect.bottom);
		LOG_USER("%s\n", strClrbg);
		m_pData->SetAttribute(L"pos", strClrbg, false);

		CRect rcFrame;
		InitDragFrame(m_pData, rcFrame);
		DM_SetVisible(true, true);
		DM_Invalidate();
	} while (false);
}


