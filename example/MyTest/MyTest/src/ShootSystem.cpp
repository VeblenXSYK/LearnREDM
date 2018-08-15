#include "StdAfx.h"
#include "CommModule.h"
#include "MainWnd.h"
#include "PreChoose.h"
#include "ExportSet.h"
#include "SceneShoot.h"
#include "SceneChoose.h"
#include "ShootSystem.h"

BEGIN_MSG_MAP(CShootSystem)
	MSG_WM_INITDIALOG(OnInitDialog)
	//MSG_WM_SIZE(OnSize)
	MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
	MSG_WM_LBUTTONUP(OnLButtonUP)
	MSG_WM_SETCURSOR(OnSetCursor)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CShootSystem)
	EVENT_NAME_COMMAND(L"closebtn", OnCloseBtn)
	EVENT_NAME_COMMAND(L"prevpagebtn", OnPrepage)
	EVENT_NAME_COMMAND(L"nextpagebtn", OnNextpage)
	EVENT_NAME_HANDLER(L"scenechoose_tree", DMEventTCSelChangedArgs::EventID, OnSceneChooseTreeSelChanged)
	EVENT_NAME_COMMAND(L"scenechoose_returnbtn", OnSceneChooseReturnBtn)
	EVENT_NAME_COMMAND(L"scenechoose_addprebtn", OnSceneChooseAddPreBtn)
	EVENT_NAME_COMMAND(L"scenechoose_delprebtn", OnSceneChooseDelPreBtn)
	EVENT_NAME_COMMAND(L"scenechoose_oneshootbtn", OnSceneChooseOneShootBtn)
	EVENT_NAME_COMMAND(L"scenechoose_allshootbtn", OnSceneChooseAllShootBtn)
	EVENT_NAME_COMMAND(L"scenedetail_returnbtn", OnSceneDetailReturnBtn)
	EVENT_NAME_COMMAND(L"scenedetail_shootbtn", OnSceneDetailShootBtn)
	EVENT_NAME_HANDLER(L"sceneshoot_lightshadeslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_contrastslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_colourtempslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_colourdiffslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_COMMAND(L"sceneshoot_returnbtn", OnSceneShootReturnBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_rotatebtn", OnSceneShootRotateBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_strawbtn", OnSceneShootStrawBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_forebtn", OnForegroundBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_exportbtn", OnExportBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_importbtn", OnImportBtn)
END_EVENT_MAP()

BOOL CShootSystem::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 获取窗口地址并保存
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"scenechoosewin"));
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"scenedetailwin"));
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"sceneshootwin"));

	// 获取标题栏地址
	m_pWndTitle = FindChildByNameT<DUIWindow>(L"scenechoosetitle");

	// 初始化场景选择
	m_pSceneChoose->Init();

	return TRUE;
}

void CShootSystem::OnSize(UINT nType, CSize size)
{
	DUIWindow *pWin = FindChildByName(L"sceneshoot_operatewin");
	if (0 != size.cx && 0 != size.cy&&pWin)
	{
		if (SIZE_MAXIMIZED == nType)
		{
			pWin->SetAttribute(L"width", L"400");
		}
		else if (SIZE_RESTORED == nType)
		{
			pWin->SetAttribute(L"width", L"300");;
		}
	}
	// 由DMHWnd继续处理OnSize消息
	SetMsgHandled(FALSE);
}

void CShootSystem::OnLButtonDbClick(UINT nFlags, CPoint pt)
{
	do
	{
		CRect titleRect;
		m_pWndTitle->DV_GetWindowRect(&titleRect);

		if (titleRect.PtInRect(pt))
		{
			SetMsgHandled(FALSE);
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
			SetActiveWindow();
		}
	} while (false);
}

void CShootSystem::OnLButtonUP(UINT nFlags, CPoint pt)
{
	if (m_strawcolor)
	{
		m_strawcolor = false;

		// 得到鼠标当前位置的颜色
		DMAutoDC hDC;

		CPoint point;
		GetCursorPos(&point);

		COLORREF RGBcolor = ::GetPixel(hDC, point.x, point.y);
		m_pSceneShoot->ShowStaticColor(RGBcolor);
	}
	SetMsgHandled(FALSE);
}

bool CShootSystem::OnSetCursor(HWND hWnd, UINT nHitTest, UINT message)
{
	if (m_strawcolor)
		::SetCursor(::LoadCursor(NULL, IDC_CROSS));
	else
		::SetCursor(::LoadCursor(NULL, IDC_ARROW));

	return true;
}

DMCode CShootSystem::OnSceneChooseTreeSelChanged(DMEventArgs *pEvt)
{
	return m_pSceneChoose->HandleTreeSelChanged(pEvt);
}

DMCode CShootSystem::OnSceneShootLightSDChanged(DMEventArgs *pEvt)
{
	m_pSceneShoot->HandleSDChanged(pEvt);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneChooseAddPreBtn()
{
	return m_pSceneChoose->HandleAddPreChoose();
}

DMCode CShootSystem::OnSceneChooseDelPreBtn()
{
	return m_pSceneChoose->HandleDelPreChoose();
}

DMCode CShootSystem::OnSceneChooseOneShootBtn()
{
	return m_pSceneChoose->HandleOneShootChoose();
}

DMCode CShootSystem::OnSceneChooseAllShootBtn()
{
	return m_pSceneChoose->HandleAllShootChoose();
}

DMCode CShootSystem::OnForegroundBtn()
{
	m_pSceneShoot->HandleForeground();

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnExportBtn()
{
	DMCode iErr = DM_ECODE_FAIL;

	do
	{
		// 创建导出窗口（模式对话框）
		DMSmartPtrT<CExportSet> pDlg; pDlg.Attach(new CExportSet(this));
		pDlg->DoModal(L"exportset", this->m_hWnd, true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode CShootSystem::OnImportBtn()
{
	m_pSceneShoot->HandleImport();

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneChooseReturnBtn()
{
	DestroyWindow();

	if (m_pMainWnd && m_pMainWnd->IsWindow())
	{
		m_pMainWnd->ShowWindow(SW_SHOW);
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneDetailReturnBtn()
{
	// 关闭“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(FALSE);
	// 显示“选择场景”窗口
	m_vecWndPtr[SCENE_CHOOSE]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneDetailShootBtn()
{
	// 关闭“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(FALSE);
	// 显示“场景拍摄”窗口
	m_vecWndPtr[SCENE_SHOOT]->DM_SetVisible(TRUE, TRUE);

	// 初始化“场景拍摄”
	m_pSceneShoot->Init();

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootReturnBtn()
{
	// 关闭“场景拍摄”窗口
	m_vecWndPtr[SCENE_SHOOT]->DM_SetVisible(FALSE);
	// 显示“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootStrawBtn()
{
	m_strawcolor = true;

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootRotateBtn()
{
	m_pSceneShoot->HandleRotate();
	return DM_ECODE_OK;
}

DMCode CShootSystem::OnPrepage()
{
	return m_pSceneChoose->HandlePrepage();
}

DMCode CShootSystem::OnNextpage()
{
	return m_pSceneChoose->HandleNextpage();
}

DMCode CShootSystem::OnCloseBtn()
{
	// 先关闭主窗口
	if (m_pMainWnd && m_pMainWnd->IsWindow())
	{
		m_pMainWnd->OnCloseBtn();
	}

	DestroyWindow();

	return DM_ECODE_OK;
}

CShootSystem::CShootSystem(CMainWnd *pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_strawcolor = false;
	m_pSceneChoose = new CSceneChoose(this);
	m_pSceneShoot = new CSceneShoot(this);
}

