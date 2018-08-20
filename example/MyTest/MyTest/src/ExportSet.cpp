#include "StdAfx.h"
#include "CommModule.h"
#include "ShootSystem.h"
#include "ExportSet.h"

BEGIN_MSG_MAP(CExportSet)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()

BEGIN_EVENT_MAP(CExportSet)
	EVENT_NAME_COMMAND(L"exportset_closebtn",OnClose)
	EVENT_NAME_COMMAND(L"exportset_confirm", OnConfirm)
	EVENT_NAME_COMMAND(L"exportset_cancel", OnCancel)
END_EVENT_MAP()

BOOL CExportSet::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	CRect rcItem;
	m_pShootSystem->DV_GetClientRect(&rcItem);

	// SetWindowPos(m_pShootSystem->m_hWnd, rcItem.left + 500, rcItem.bottom - 200, 0, 0, SWP_NOSIZE);
	MoveWindow(rcItem.right - 300, rcItem.bottom - 400, 280, 300);

	return TRUE;
}

void CExportSet::OnSize(UINT nType, CSize size)
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

DMCode CExportSet::OnClose()
{
	EndDialog(IDCLOSE);
	return DM_ECODE_OK;
}

DMCode CExportSet::OnConfirm()
{
	std::string outMsg;

	// 导出PSD格式
	if (FindChildByNameT<DUICheckBox>(L"exportset_psdcheckbox")->DM_IsChecked())
	{
		CStringW strCWPath;
		strCWPath.Format(L"%sExport\\test.psd", CCommModule::GetPicRootDir());
		std::wstring strWPath = (LPCWSTR)strCWPath;
		int ret = PSExportPSDFile(CCommModule::GetPSHandle(), CCommModule::GetRawString(CCommModule::WS2S(strWPath)), outMsg);
	}

	// 导出JPG格式
	if (FindChildByNameT<DUICheckBox>(L"exportset_jpgcheckbox")->DM_IsChecked())
	{
		CStringW strCWPath;
		strCWPath.Format(L"%sExport\\test.jpg", CCommModule::GetPicRootDir());
		std::wstring strWPath = (LPCWSTR)strCWPath;
		int ret = PSExportJPGFile(CCommModule::GetPSHandle(), CCommModule::GetRawString(CCommModule::WS2S(strWPath)), outMsg);
	}

	EndDialog(IDOK);
	return DM_ECODE_OK;
}

DMCode CExportSet::OnCancel()
{
	EndDialog(IDCANCEL);
	return DM_ECODE_OK;
}

CExportSet::CExportSet(CShootSystem *pShootSystem)
{
	m_pShootSystem = pShootSystem;
}

