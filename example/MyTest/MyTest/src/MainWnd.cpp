#include "StdAfx.h"
#include "MainWnd.h"

BEGIN_MSG_MAP(CMainWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
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
	// 由DMHWnd继续处理OnSize消息
	SetMsgHandled(FALSE);
}

DMCode CMainWnd::OnClose()
{
	// 所有关闭窗口统一使用DestroyWindow()，不要使用PostMessage(WM_QUIT)
	DestroyWindow();
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnShootSystem()
{
	do
	{
		if (m_pShootSystem && m_pShootSystem->IsWindow())
		{
			break;
		}
		m_pShootSystem.Release();
		m_pShootSystem.Attach(new CShootSystem(this));

		// 创建拍摄系统窗口
		m_pShootSystem->DM_CreateWindow(L"shootsystem", 0, 0, 0, 0, false);
		m_pShootSystem->SendMessage(WM_INITDIALOG);
		m_pShootSystem->CenterWindow();
		m_pShootSystem->ShowWindow(SW_SHOW);

		// 隐藏主窗口
		this->ShowWindow(SW_HIDE);
	} while (false);

	return DM_ECODE_OK;
}

DMCode CMainWnd::OnUploadSystem()
{
	return DM_ECODE_OK;
}
