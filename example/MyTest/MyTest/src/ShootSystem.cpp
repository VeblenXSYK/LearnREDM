#include "StdAfx.h"
#include "CommModule.h"
#include "MainWnd.h"
#include "PreChoose.h"
#include "ExportSet.h"
#include "SceneShoot.h"
#include "SceneChoose.h"
#include "BackgroundPreview.h"
#include "ShootSystem.h"

BEGIN_MSG_MAP(CShootSystem)
	MSG_WM_INITDIALOG(OnInitDialog)
	// MSG_WM_SIZE(OnSize)
	// MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
	MSG_WM_LBUTTONUP(OnLButtonUP)
	MSG_WM_SETCURSOR(OnSetCursor)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CShootSystem)
	EVENT_NAME_COMMAND(L"closebtn", OnCloseBtn)
	EVENT_NAME_HANDLER(L"scenechoose_tree", DMEventTCSelChangedArgs::EventID, OnSceneChooseTreeSelChanged)
	EVENT_NAME_COMMAND(L"scenechoose_returnbtn", OnSceneChooseReturnBtn)
	EVENT_NAME_COMMAND(L"scenechoose_addprebtn", OnSceneChooseAddPreBtn)
	EVENT_NAME_COMMAND(L"scenechoose_delprebtn", OnSceneChooseDelPreBtn)
	EVENT_NAME_COMMAND(L"scenechoose_oneshootbtn", OnSceneChooseOneShootBtn)
	EVENT_NAME_COMMAND(L"scenechoose_allshootbtn", OnSceneChooseAllShootBtn)
	EVENT_NAME_COMMAND(L"scenedetail_returnbtn", OnSceneDetailReturnBtn)
	EVENT_NAME_COMMAND(L"scenedetail_shootbtn", OnSceneDetailShootBtn)
	EVENT_NAME_COMMAND(L"scenedetail_prevpagebtn", OnSceneDetailPrevpageBtn)
	EVENT_NAME_COMMAND(L"scenedetail_nextpagebtn", OnSceneDetailNextpageBtn)
	/*EVENT_NAME_HANDLER(L"sceneshoot_lightshadeslider", DMEventSDChangingArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_contrastslider", DMEventSDChangingArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_colourtempslider", DMEventSDChangingArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_colourdiffslider", DMEventSDChangingArgs::EventID, OnSceneShootLightSDChanged)*/
	EVENT_NAME_HANDLER(L"sceneshoot_lightshadeslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_contrastslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_colourtempslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_HANDLER(L"sceneshoot_colourdiffslider", DMEventSDChangedArgs::EventID, OnSceneShootLightSDChanged)
	EVENT_NAME_COMMAND(L"sceneshoot_prevpagebtn", OnSceneShootPrevpageBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_nextpagebtn", OnSceneShootNextpageBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_returnbtn", OnSceneShootReturnBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_rotatebtn", OnSceneShootRotateBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_strawbtn", OnSceneShootStrawBtn)
	EVENT_NAME_COMMAND(L"sceneshoot_addenvircolorbtn", OnSceneShootEnvirColorBtn)
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

	// “正在加载”的gif对象
	m_pLoadingGif = FindChildByNameT<DUIGif>(L"loadinggif");

	// “场景详情”图片窗口
	m_pImageWin = FindChildByNameT<DUIWindow>(L"scenedetail_imagewin");
	// “场景详情”背景框
	m_pBackground = FindChildByNameT<CBackgroundPreview>(L"scenedetail_previewbg");

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
	SetLoadingState();
	DMCode ret = m_pSceneChoose->HandleOneShootChoose();
	if (ret == DM_ECODE_FAIL)
		ClearLoadingState();
	return ret;
}

DMCode CShootSystem::OnSceneChooseAllShootBtn()
{
	SetLoadingState();
	DMCode ret = m_pSceneChoose->HandleAllShootChoose();
	if (ret == DM_ECODE_FAIL)
		ClearLoadingState();
	return ret;
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
	m_pSceneChoose->ProduceParsePsdMessage(m_pSceneChoose->m_curImagepath, CGuiMessage::SECENEDETAIL_PARSEPSD_SHOOT);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneDetailPrevpageBtn()
{
	SetLoadingState();
	return m_pSceneChoose->HandlePrevpage(SCENE_DETAIL);
}

DMCode CShootSystem::OnSceneDetailNextpageBtn()
{
	SetLoadingState();
	return m_pSceneChoose->HandleNextpage(SCENE_DETAIL);
}

DMCode CShootSystem::OnSceneShootPrevpageBtn()
{
	SetLoadingState();
	return m_pSceneChoose->HandlePrevpage(SCENE_SHOOT);
}

DMCode CShootSystem::OnSceneShootNextpageBtn()
{
	SetLoadingState();
	return m_pSceneChoose->HandleNextpage(SCENE_SHOOT);
}

DMCode CShootSystem::OnSceneShootReturnBtn()
{
	// 释放“场景拍摄”中的人物控件
	m_pSceneShoot->HandleReturnBtn();

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

DMCode CShootSystem::OnSceneShootEnvirColorBtn()
{
	m_pSceneShoot->HandleEnvirColor();
	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootRotateBtn()
{
	m_pSceneShoot->HandleRotate();
	return DM_ECODE_OK;
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

void CShootSystem::SetLoadingState()
{
	m_pLoadingGif->DM_SetVisible(TRUE, true);
}

void CShootSystem::ClearLoadingState()
{
	m_pLoadingGif->DM_SetVisible(FALSE, true);
}

void CShootSystem::ChangeSceneDetailBg(std::string &imgBuf)
{
	// 更改背景图片
	DMSmartPtrT<IDMSkin> pSkinBg = g_pDMApp->GetSkin(L"scenedetailstatic1bg");
	pSkinBg->SetBitmap((LPBYTE)imgBuf.c_str(), imgBuf.size(), L"");
	FindChildByNameT<DUIStatic>(L"scenedetail_static1bg")->DM_Invalidate();
}

void CShootSystem::ChangeSceneDetailPreviewBg(std::wstring &filepath)
{
	const wchar_t *szFilePath = filepath.c_str();
	size_t ulSize = DM::GetFileSizeW(szFilePath);

	DWORD dwReadSize = 0;
	DMBufT<byte>pBuf; pBuf.Allocate(ulSize);
	DM::GetFileBufW(szFilePath, (void **)&pBuf, ulSize, dwReadSize);

	// 创建预览skin
	DMSmartPtrT<IDMSkin> pSkin;
	g_pDMApp->CreateRegObj((void**)&pSkin, L"imglist", DMREG_Skin);
	pSkin->SetBitmap(pBuf, ulSize, L"png");
	m_pBackground->m_pSkin = pSkin;

	// 刷新
	m_pImageWin->DM_Invalidate();
}

