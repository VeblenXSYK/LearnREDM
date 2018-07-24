#include "StdAfx.h"
#include "MainWnd.h"
#include "ImagePreview.h"
#include "ShootSystem.h"

#include <atlconv.h>
#include <direct.h>
#include <io.h>
#include <fstream>

BEGIN_MSG_MAP(CShootSystem)
	MSG_WM_INITDIALOG(OnInitDialog)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CShootSystem)
	EVENT_NAME_HANDLER(L"scenechoose_tree", DMEventTCSelChangedArgs::EventID, OnTreeSelChanged)
	EVENT_NAME_COMMAND(L"returnbtn", OnReturn)
	EVENT_NAME_COMMAND(L"prevpagebtn", OnPrepage)
	EVENT_NAME_COMMAND(L"nextpagebtn", OnNextpage)
	EVENT_NAME_COMMAND(L"closebtn", OnClose)
END_EVENT_MAP()


BOOL CShootSystem::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 获取窗口标题
	//pWndTitle = FindChildByNameT<DUIEdit>(L"wndtitle");
	pWndTitle = FindChildByNameT<DUIWindow>(L"wndtitle");

	// 获取窗口地址并保存
	pSceneChooseWnd = FindChildByNameT<DUIWindow>(L"scenechoosewin");
	if (pSceneChooseWnd != NULL)
	{
		m_vecWndPtr.push_back(pSceneChooseWnd);
		m_vecWndTitle.push_back(L"选择场景");
	}

	pSceneDetailWnd = FindChildByNameT<DUIWindow>(L"scenedetailwin");
	if (pSceneDetailWnd != NULL)
	{ 
		m_vecWndPtr.push_back(pSceneDetailWnd);
		m_vecWndTitle.push_back(L"场景详情");
	}

	pSceneShootWnd = FindChildByNameT<DUIWindow>(L"sceneshootwin");
	if (pSceneShootWnd != NULL)
	{ 
		m_vecWndPtr.push_back(pSceneShootWnd);
		m_vecWndTitle.push_back(L"场景拍摄");
	}

	// 设置当前显示的窗口
	m_curWndType = SCENE_CHOOSE;

	// 获取控件名称
	m_pTreeCtrl = FindChildByNameT<DUITreeCtrl>(L"scenechoose_tree");
	pWrapLayout = FindChildByNameT<DUIWindow>(L"wraplayout", TRUE);
	pListBoxEx = FindChildByNameT<DUIListBoxEx>(L"scenechoose_listboxex");
	pListBoxEx->SetItemHeight(0, 800, true);

	// 获取图片目录路径
	GetRootFullPath(L".\\WorldTripShootRes\\outimage\\European", m_wcPicDirPath_European, MAX_PATH);
	GetRootFullPath(L".\\WorldTripShootRes\\outimage\\Pastoral", m_wcPicDirPath_Pastoral, MAX_PATH);

	// 保存目录下所有的文件路径
	GetFilePathOfFmt(m_wcPicDirPath_European, m_vecPicPath_European, L"png");
	GetFilePathOfFmt(m_wcPicDirPath_Pastoral, m_vecPicPath_Pastoral, L"png");

	// 显示欧式简约图片
	m_curShowPicType = EUROPEAN;
	ShowImageOfWrap(0);

	return TRUE;
}

DMCode CShootSystem::OnTreeSelChanged(DMEventArgs *pEvt)
{
	DMEventTCSelChangedArgs *pSelEvt = (DMEventTCSelChangedArgs*)pEvt;
	HDMTREEITEM hSelItem = pSelEvt->m_hNewSel;
	if (hSelItem)
	{
		CStringW text = m_pTreeCtrl->GetItemText(hSelItem);
		 if (text == L"欧式简约")
		{
			m_curShowPicType = EUROPEAN;

			ClearImageOfWrap();
			ShowImageOfWrap(0);
			m_curPageNum_Wrap = 0;
		}
		else if (text == L"唯美田园")
		{
			m_curShowPicType = PASTORAL;

			ClearImageOfWrap();
			ShowImageOfWrap(0);
			m_curPageNum_Wrap = 0;
		}
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnReturn()
{
	DestroyWindow();

	if (m_pMainWnd && m_pMainWnd->IsWindow())
	{
		m_pMainWnd->ShowWindow(SW_SHOW);
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnPrepage()
{
	// 设置当前窗口不可见
	m_vecWndPtr[m_curWndType]->DM_SetVisible(FALSE);

	// 重新设置当前窗口
	m_curWndType = m_curWndType - 1;
	if (m_curWndType == -1)
	{
		m_curWndType = SCENE_UNDEF - 1;
	}
	m_vecWndPtr[m_curWndType]->DM_SetVisible(TRUE, TRUE);
	pWndTitle->DV_SetWindowText(m_vecWndTitle[m_curWndType].c_str());

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnNextpage()
{
	m_vecWndPtr[m_curWndType]->DM_SetVisible(FALSE);

	m_curWndType = m_curWndType + 1;
	if (m_curWndType == SCENE_UNDEF)
	{
		m_curWndType = SCENE_CHOOSE;
	}
	m_vecWndPtr[m_curWndType]->DM_SetVisible(TRUE, TRUE);
	pWndTitle->DV_SetWindowText(m_vecWndTitle[m_curWndType].c_str());

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
}

// 显示WrapLayout中的图片
void CShootSystem::ShowImageOfWrap(int sindex)
{
	std::vector<std::wstring> files;
	if (m_curShowPicType == EUROPEAN)
		files = m_vecPicPath_European;
	else
		files = m_vecPicPath_Pastoral;

	int filenum = files.size();
	int eindex;
	if (sindex + PicNumOfPage > filenum - 1)
		eindex = filenum - 1;
	else
		eindex = sindex + PicNumOfPage;

	for (int i = sindex; i < eindex; i++)
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

