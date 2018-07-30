#include "StdAfx.h"
#include "MainWnd.h"
#include "ImagePreview.h"
#include "PreChoose.h"
#include "ExportSet.h"
#include "SceneShoot.h"
#include "ShootSystem.h"

#include <atlconv.h>
#include <direct.h>
#include <io.h>
#include <fstream>

BEGIN_MSG_MAP(CShootSystem)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CShootSystem)
	EVENT_NAME_HANDLER(L"scenechoose_tree", DMEventTCSelChangedArgs::EventID, OnTreeSelChanged)
	EVENT_NAME_HANDLER(L"lightshadeslider", DMEventSDChangedArgs::EventID, OnLightSDChanged)
	EVENT_NAME_HANDLER(L"contrastslider", DMEventSDChangedArgs::EventID, OnLightSDChanged)
	EVENT_NAME_HANDLER(L"colourtempslider", DMEventSDChangedArgs::EventID, OnLightSDChanged)
	EVENT_NAME_HANDLER(L"colourdiffslider", DMEventSDChangedArgs::EventID, OnLightSDChanged)
	EVENT_NAME_COMMAND(L"closebtn", OnClose)
	EVENT_NAME_COMMAND(L"scenechoose_returnbtn", OnSceneChooseReturn)
	EVENT_NAME_COMMAND(L"scenechoose_addbtn", OnAddPreChoose)
	EVENT_NAME_COMMAND(L"scenechoose_delbtn", OnDelPreChoose)
	EVENT_NAME_COMMAND(L"scenechoose_onebtn", OnOneShootChoose)
	EVENT_NAME_COMMAND(L"scenedetail_returnbtn", OnSceneDetailReturn)
	EVENT_NAME_COMMAND(L"scenedetail_shootbtn", OnSceneDetailShoot)
	EVENT_NAME_COMMAND(L"sceneshoot_returnbtn", OnSceneShootReturn)
	EVENT_NAME_COMMAND(L"sceneshoot_forebtn", OnForeground)
	EVENT_NAME_COMMAND(L"sceneshoot_exportbtn", OnExport)
	EVENT_NAME_COMMAND(L"sceneshoot_importbtn", OnImport)
	EVENT_NAME_COMMAND(L"prevpagebtn", OnPrepage)
	EVENT_NAME_COMMAND(L"nextpagebtn", OnNextpage)
END_EVENT_MAP()


BOOL CShootSystem::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 获取窗口地址并保存
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"scenechoosewin"));
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"scenedetailwin"));
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"sceneshootwin"));

	// 获取控件名称
	m_pTreeCtrl = FindChildByNameT<DUITreeCtrl>(L"scenechoose_tree");
	pWrapLayout = FindChildByNameT<DUIWindow>(L"wraplayout", TRUE);
	pListBoxEx = FindChildByNameT<DUIListBoxEx>(L"scenechoose_listboxex");

	// 获取图片目录路径
	GetRootFullPath(L".\\WorldTripShootRes\\outimage\\European", m_wcPicDirPath_European, MAX_PATH);
	GetRootFullPath(L".\\WorldTripShootRes\\outimage\\Pastoral", m_wcPicDirPath_Pastoral, MAX_PATH);

	// 保存目录下所有的文件路径
	GetFilePathOfFmt(m_wcPicDirPath_European, m_vecPicPath_European, L"png");
	GetFilePathOfFmt(m_wcPicDirPath_Pastoral, m_vecPicPath_Pastoral, L"png");

	// 显示欧式简约图片
	m_curShowPicType = EUROPEAN;
	ShowImageOfWrap();

	return TRUE;
}

void CShootSystem::OnLButtonDbClick(UINT nFlags, CPoint pt)
{
	do
	{
		SetMsgHandled(FALSE);
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
		SetActiveWindow();
	} while (false);
}

DMCode CShootSystem::OnTreeSelChanged(DMEventArgs *pEvt)
{
	DMEventTCSelChangedArgs *pSelEvt = (DMEventTCSelChangedArgs*)pEvt;
	HDMTREEITEM hSelItem = pSelEvt->m_hNewSel;
	m_hSelItem_tree = hSelItem;
	if (hSelItem)
	{
		CStringW text = m_pTreeCtrl->GetItemText(hSelItem);
		 if (text == L"欧式简约")
		{
			m_curShowPicType = EUROPEAN;

			ClearImageOfWrap();
			ShowImageOfWrap();
			m_curPageNum_Wrap = 0;
		}
		else if (text == L"唯美田园")
		{
			m_curShowPicType = PASTORAL;

			ClearImageOfWrap();
			ShowImageOfWrap();
			m_curPageNum_Wrap = 0;
		}
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnLightSDChanged(DMEventArgs *pEvt)
{
	pSceneShoot->HandleSDChanged(pEvt);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnAddPreChoose()
{
	DMCode iErr = DM_ECODE_FAIL;

	do
	{
		// 创建预选窗口（模式对话框）
		DMSmartPtrT<CPreChoose> pDlg; pDlg.Attach(new CPreChoose(this));
		pDlg->DoModal(L"prechoose", this->m_hWnd, true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode CShootSystem::OnDelPreChoose()
{
	if (m_hSelItem_tree != NULL)
	{
		// 只有父项为“预选”的项才能被删除
		HDMTREEITEM hParentItem = m_pTreeCtrl->GetParentItem(m_hSelItem_tree);
		if (m_pTreeCtrl->GetItemText(hParentItem) == L"预选")
		{
			m_pTreeCtrl->RemoveItem(m_hSelItem_tree);
			m_hSelItem_tree = NULL;
		}
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnOneShootChoose()
{
	// 关闭“选择场景”窗口
	m_vecWndPtr[SCENE_CHOOSE]->DM_SetVisible(FALSE);

	// 显示“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnForeground()
{
	pSceneShoot->HandleForeground();

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnExport()
{
	DMCode iErr = DM_ECODE_FAIL;

	do
	{
		// 创建预选窗口（模式对话框）
		DMSmartPtrT<CExportSet> pDlg; pDlg.Attach(new CExportSet(this));
		pDlg->DoModal(L"exportset", this->m_hWnd, true);

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode CShootSystem::OnImport()
{
	pSceneShoot->HandleImport();

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneChooseReturn()
{
	DestroyWindow();

	if (m_pMainWnd && m_pMainWnd->IsWindow())
	{
		m_pMainWnd->ShowWindow(SW_SHOW);
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneDetailReturn()
{
	// 关闭“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(FALSE);

	// 显示“选择场景”窗口
	m_vecWndPtr[SCENE_CHOOSE]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneDetailShoot()
{
	// 关闭“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(FALSE);

	// 显示“场景拍摄”窗口
	m_vecWndPtr[SCENE_SHOOT]->DM_SetVisible(TRUE, TRUE);

	// 初始化“场景拍摄”
	pSceneShoot->Init();

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootReturn()
{
	// 关闭“场景拍摄”窗口
	m_vecWndPtr[SCENE_SHOOT]->DM_SetVisible(FALSE);

	// 显示“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnPrepage()
{
	return DM_ECODE_OK;
}

DMCode CShootSystem::OnNextpage()
{
	return DM_ECODE_OK;
}

DMCode CShootSystem::OnClose()
{
	if (m_pMainWnd && m_pMainWnd->IsWindow())
	{
		m_pMainWnd->OnClose();
	}

	DestroyWindow();

	return DM_ECODE_OK;
}

CShootSystem::CShootSystem(CMainWnd *pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_curPageNum_Wrap = 0;
	m_hSelItem_tree = NULL;
	pSceneShoot = new CSceneShoot(this);
}

// 显示WrapLayout中的图片
void CShootSystem::ShowImageOfWrap()
{
	std::vector<std::wstring> files;
	if (m_curShowPicType == EUROPEAN)
		files = m_vecPicPath_European;
	else
		files = m_vecPicPath_Pastoral;

	int filenum = files.size();
	for (int i = 0; i < filenum; i++)
	{
		const wchar_t *szFilePath = files[i].c_str();
		size_t ulSize = DM::GetFileSizeW(szFilePath);

		DWORD dwReadSize = 0;
		DMBufT<byte>pBuf; pBuf.Allocate(ulSize);
		DM::GetFileBufW(szFilePath, (void **)&pBuf, ulSize, dwReadSize);

		// 创建预览skin
		DMSmartPtrT<IDMSkin> pSkin;
		g_pDMApp->CreateRegObj((void**)&pSkin, L"imglist", DMREG_Skin);
		pSkin->SetBitmap(pBuf, ulSize, L"png");
		
		// 创建ImagePreview子控件
		ImagePreview *pChild = NULL;
		g_pDMApp->CreateRegObj((void**)&pChild, L"ImagePreview", DMREG_Window);
		if (pChild)
		{
			pChild->m_pSkin = pSkin;
			pWrapLayout->DM_InsertChild(pChild);
			m_vecChildPtr_Wrap.push_back(pChild);
		}
	}

	// 根据需要调整ListBoxEx的高度
	if (ceil(filenum / 5.0) * PICHEIGHT_ONE > PICHEIGHT_AREA)
		pListBoxEx->SetItemHeight(0, ceil(filenum / 5.0) * PICHEIGHT_ONE, true);

	pWrapLayout->DV_UpdateChildLayout();
}

// 清空WrapLayout中的图片
void CShootSystem::ClearImageOfWrap(void)
{
	while (!m_vecChildPtr_Wrap.empty())
	{
		ImagePreview *pChild = m_vecChildPtr_Wrap.back();
		if (pChild != NULL)
		{
			pWrapLayout->DM_DestroyChildWnd(pChild);
			m_vecChildPtr_Wrap.pop_back();
		}
	}
}

//void CShootSystem::ShowImage(const wchar_t *pFilePath, const wchar_t *lpszName, LPCWSTR lpszSkin)
//{
//	DWORD dwSize = DM::GetFileSizeW(pFilePath);
//	if (dwSize != 0)
//	{
//		DMBufT<byte>pBuf; pBuf.Allocate(dwSize);
//		DWORD dwRead = 0;
//		DM::GetFileBufW(pFilePath, (void**)&pBuf, dwSize, dwRead);
//
//		DMSmartPtrT<IDMSkin> pSkin = g_pDMApp->GetSkin(lpszSkin);
//		if (pSkin)
//		{// 如果skin存在，则直接删除
//#if 0// 如果skin存在，则直接改内存
//			DMSmartPtrT<IDMBitmap> pBitmap;
//			pSkin->GetBitmap(&pBitmap);
//			if (pBitmap)
//			{
//				pBitmap->LoadFromMemory(pBuf, dwSize, L"");
//				FindChildByNameT<DUIButton>(L"closebtn")->DM_Invalidate();
//			}
//			break;
//#else
//			g_pDMApp->RemoveSkin(lpszSkin, L"ccskinpool");
//#endif
//		}
//		// 如果skin不存在，则创建skin
//		CStringW strWXml;
//		strWXml.Format(L"<imglist id=\"%s\" states=\"3\"/>", lpszSkin);								// xml中不要配src了		
//		g_pDMApp->AddSkin(pBuf, dwSize, L"png", strWXml, L"ccskinpool");							// 加入到ccskinpool这个skin池
//		
//		CStringW strWXml2;
//		strWXml2.Format(L"<root><ImagePreview skin=\"%s\"/></root>", lpszSkin);
//		CStringA strXml = DMW2A(strWXml2, CP_UTF8);
//		DMXmlDocument doc;
//		doc.LoadFromBuffer((const PVOID)(LPCSTR)strXml, strXml.GetLength());
//		DMXmlNode ImageNode = doc.Root();
//
//		pWrapLayout->DV_CreateChildWnds(ImageNode);
//	}
//}

// 获取指定目录下特定格式的所有文件路径
void CShootSystem::GetFilePathOfFmt(std::wstring dirpath, std::vector<std::wstring> &files, std::wstring fmt)
{
	//文件句柄
	long hFile = 0;

	//文件信息
	struct _wfinddata_t fileinfo;
	std::wstring p;
	if ((hFile = _wfindfirst(p.assign(dirpath).append(L"\\*" + fmt).c_str(), &fileinfo)) != -1)
	{
		do
		{
			files.push_back(p.assign(dirpath).append(L"\\").append(fileinfo.name));
		} while (_wfindnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

