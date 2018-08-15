#include "StdAfx.h"
#include "MainWnd.h"

BEGIN_MSG_MAP(CMainWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()

BEGIN_EVENT_MAP(CMainWnd)
	EVENT_NAME_COMMAND(L"closebtn",OnCloseBtn)
	EVENT_NAME_COMMAND(L"shootsystembtn", OnShootSystemBtn)
	EVENT_NAME_COMMAND(L"uploadsystembtn", OnUploadSystemBtn)
END_EVENT_MAP()

BOOL CMainWnd::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	return TRUE;
}

DMCode CMainWnd::OnCloseBtn()
{
	// 所有关闭窗口统一使用DestroyWindow()，不要使用PostMessage(WM_QUIT)
	DestroyWindow();
	return DM_ECODE_OK;
}

DMCode CMainWnd::OnShootSystemBtn()
{
	do
	{
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

DMCode CMainWnd::OnUploadSystemBtn()
{
	return DM_ECODE_OK;
}
