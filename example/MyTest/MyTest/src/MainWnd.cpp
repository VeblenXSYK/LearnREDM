#include "StdAfx.h"
#include "MainWnd.h"

BEGIN_MSG_MAP(CMainWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_LBUTTONDOWN(OnLeftClickBtn)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CMainWnd)
	EVENT_NAME_COMMAND(L"loginclosebtn",OnClose)
	EVENT_NAME_COMMAND(L"loginshootbtn", OnShootSystem)
	EVENT_NAME_COMMAND(L"loginuploadbtn", OnUploadSystem)

END_EVENT_MAP()

BOOL CMainWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	return TRUE;
}

/*
	处理鼠标左键点击事件
*/
void CMainWnd::OnLeftClickBtn(UINT nFlags, CPoint pt)
{
	// DUIWindow* pLoginShootBtn = FindChildByName(L"loginshootbtn");
	// pLoginShootBtn->DV_SetWindowText(L"nimmei");
	SetMsgHandled(FALSE);  // 由DMHWnd继续处理OnLeftClickBtn消息
}

void CMainWnd::OnSize(UINT nType, CSize size)
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
	SetMsgHandled(FALSE);  // 由DMHWnd继续处理OnSize消息
}

DMCode CMainWnd::OnClose()
{
	DestroyWindow();		// 所有关闭窗口统一使用DestroyWindow()，不要使用PostMessage(WM_QUIT)
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnShootSystem()
{
	do
	{
		if (m_pSceneChoose && m_pSceneChoose->IsWindow())
		{
			break;
		}
		m_pSceneChoose.Release();
		m_pSceneChoose.Attach(new CSceneChoose(this));

		// 创建主窗口
		m_pSceneChoose->DM_CreateWindow(L"sceneselect", 0, 0, 0, 0, m_hWnd);
		m_pSceneChoose->SendMessage(WM_INITDIALOG);
		m_pSceneChoose->CenterWindow();
		m_pSceneChoose->ShowWindow(SW_SHOW);

		DUIWindow* pLoginShootBtn = FindChildByName(L"loginshootbtn");
		//pLoginShootBtn->DV_SetWindowText(L"nimmei");

		// 隐藏原来的窗口
		this->ShowWindow(SW_HIDE);
	} while (false);

	return DM_ECODE_OK;
}

DMCode CMainWnd::OnUploadSystem()
{
	return DM_ECODE_OK;
}
