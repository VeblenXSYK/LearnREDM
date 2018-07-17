#include "StdAfx.h"
#include "MainWnd.h"
#include "SceneChoose.h"

BEGIN_MSG_MAP(CSceneChoose)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CSceneChoose)
	EVENT_NAME_COMMAND(L"scenechoose_closebtn", OnClose)
	EVENT_NAME_COMMAND(L"scenechoose_retbtn", OnReturn)
END_EVENT_MAP()

BOOL CSceneChoose::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	return TRUE;
}

DMCode CSceneChoose::OnClose()
{
	DestroyWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);
	return DM_ECODE_OK;
}

DMCode CSceneChoose::OnReturn()
{
	return DM_ECODE_OK;
}

CSceneChoose::CSceneChoose(CMainWnd *pMainWnd)
{
	m_pMainWnd = pMainWnd;
}