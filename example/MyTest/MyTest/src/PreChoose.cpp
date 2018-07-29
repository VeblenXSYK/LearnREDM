#include "StdAfx.h"
#include "PreChoose.h"
#include "ShootSystem.h"

BEGIN_MSG_MAP(CPreChoose)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()

BEGIN_EVENT_MAP(CPreChoose)
	EVENT_NAME_COMMAND(L"addprechoose_closebtn",OnClose)
	EVENT_NAME_COMMAND(L"addprechoose_confirm", OnConfirm)
	EVENT_NAME_COMMAND(L"addprechoose_cancel", OnCancel)
END_EVENT_MAP()

BOOL CPreChoose::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	CRect rcItem;
	m_pShootSystem->DV_GetClientRect(&rcItem);

	// SetWindowPos(m_pShootSystem->m_hWnd, rcItem.left + 500, rcItem.bottom - 200, 0, 0, SWP_NOSIZE);
	MoveWindow(rcItem.left + 500, rcItem.bottom - 200, 350, 150);

	pNameEdit = FindChildByNameT<DUIEdit>(L"prechoose_nameedit");

	return TRUE;
}

void CPreChoose::OnSize(UINT nType, CSize size)
{
	DUIWindow* pMaxBtn = FindChildByName(L"maxbutton");
	DUIWindow* pRestoreBtn = FindChildByName(L"restorebutton");
	if (0 != size.cx&&0 != size.cy&&pMaxBtn&&pRestoreBtn)
	{
		if (SIZE_MAXIMIZED == nType)
		{
			pMaxBtn->DM_SetVisible(false);
			pRestoreBtn->DM_SetVisible(true);
		}
		else if (SIZE_RESTORED == nType)
		{
			pMaxBtn->DM_SetVisible(true);
			pRestoreBtn->DM_SetVisible(false);
		}
	}
	// 由DMHWnd继续处理OnSize消息
	SetMsgHandled(FALSE);
}

DMCode CPreChoose::OnClose()
{
	EndDialog(IDCLOSE);
	return DM_ECODE_OK;
}

DMCode CPreChoose::OnConfirm()
{
	DUITreeCtrl *pTreeCtrl = m_pShootSystem->m_pTreeCtrl;
	HDMTREEITEM hTree = pTreeCtrl->GetRootItem();
	while (hTree != NULL)
	{
		if (pTreeCtrl->GetItemText(hTree) == L"预选")
		{
			// 获取设置名称
			CStringW name =pNameEdit->GetWindowText().Trim();

			// 检查是否已经存在
			bool exist = false;
			HDMTREEITEM hchild = pTreeCtrl->GetChildItem(hTree);
			while (hchild != NULL)
			{
				if (pTreeCtrl->GetItemText(hchild) == name)
				{
					exist = true;
					break;
				}

				hchild = pTreeCtrl->GetNextSiblingItem(hchild);
			}

			if(!exist)
				pTreeCtrl->InsertItem(name, 1, 1, (LPARAM)-1, hTree);
		}

		hTree = pTreeCtrl->GetNextSiblingItem(hTree);
	}

	EndDialog(IDOK);
	return DM_ECODE_OK;
}

DMCode CPreChoose::OnCancel()
{
	EndDialog(IDCANCEL);
	return DM_ECODE_OK;
}

CPreChoose::CPreChoose(CShootSystem *pShootSystem)
{
	m_pShootSystem = pShootSystem;
}

