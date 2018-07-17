#include "StdAfx.h"
#include "MainWnd.h"

BEGIN_MSG_MAP(CMainWnd)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_LBUTTONDOWN(OnLeftClickBtn)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHWnd)// ��δ�������Ϣ����DMHWnd����
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
	��������������¼�
*/
void CMainWnd::OnLeftClickBtn(UINT nFlags, CPoint pt)
{
	// DUIWindow* pLoginShootBtn = FindChildByName(L"loginshootbtn");
	// pLoginShootBtn->DV_SetWindowText(L"nimmei");
	SetMsgHandled(FALSE);  // ��DMHWnd��������OnLeftClickBtn��Ϣ
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
	SetMsgHandled(FALSE);  // ��DMHWnd��������OnSize��Ϣ
}

DMCode CMainWnd::OnClose()
{
	DestroyWindow();		// ���йرմ���ͳһʹ��DestroyWindow()����Ҫʹ��PostMessage(WM_QUIT)
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

		// ����������
		m_pSceneChoose->DM_CreateWindow(L"sceneselect", 0, 0, 0, 0, m_hWnd);
		m_pSceneChoose->SendMessage(WM_INITDIALOG);
		m_pSceneChoose->CenterWindow();
		m_pSceneChoose->ShowWindow(SW_SHOW);

		DUIWindow* pLoginShootBtn = FindChildByName(L"loginshootbtn");
		//pLoginShootBtn->DV_SetWindowText(L"nimmei");

		// ����ԭ���Ĵ���
		this->ShowWindow(SW_HIDE);
	} while (false);

	return DM_ECODE_OK;
}

DMCode CMainWnd::OnUploadSystem()
{
	return DM_ECODE_OK;
}
