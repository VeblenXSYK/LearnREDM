#include "StdAfx.h"
#include "DUIStatic.h"
#include "DUIEdit.h"

#include "MainWnd.h"
#include "ImagePreview.h"
#include "SceneChoose.h"

#include <atlconv.h>
#include <direct.h>
#include <io.h>
#include <fstream>

BEGIN_MSG_MAP(CSceneChoose)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_SIZE(OnSize)
	CHAIN_MSG_MAP(DMHWnd)// 将未处理的消息交由DMHWnd处理
END_MSG_MAP()
BEGIN_EVENT_MAP(CSceneChoose)
	EVENT_NAME_HANDLER(L"scenechoose_tree", DMEventTCSelChangedArgs::EventID, OnTreeSelChanged)
	EVENT_NAME_COMMAND(L"scenechoose_retbtn", OnReturn)
	EVENT_NAME_COMMAND(L"scenechoose_prepagebtn", OnPrepage)
	EVENT_NAME_COMMAND(L"scenechoose_nextpagebtn", OnNextpage)
	EVENT_NAME_COMMAND(L"scenechoose_closebtn", OnClose)
END_EVENT_MAP()


BOOL CSceneChoose::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// 获取窗口的名称
	m_pTreeCtrl = FindChildByNameT<DUITreeCtrl>(L"scenechoose_tree");
	pWrapLayout = FindChildByNameT<DUIWindow>(L"wraplayout");

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

DMCode CSceneChoose::OnTreeSelChanged(DMEventArgs *pEvt)
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

DMCode CSceneChoose::OnReturn()
{
	DestroyWindow();
	m_pMainWnd->ShowWindow(SW_SHOW);

	return DM_ECODE_OK;
}

DMCode CSceneChoose::OnPrepage()
{
	if (m_curPageNum_Wrap != 0)
	{
		unsigned int preStart = (m_curPageNum_Wrap - 1) * PicNumOfPage;

		ClearImageOfWrap();
		ShowImageOfWrap(preStart);

		m_curPageNum_Wrap--;
	}

	return DM_ECODE_OK;
}

DMCode CSceneChoose::OnNextpage()
{
	unsigned int nextStart = (m_curPageNum_Wrap + 1) * PicNumOfPage;
	if (nextStart < m_vecPicPath_European.size())
	{
		ClearImageOfWrap();
		ShowImageOfWrap(nextStart);

		m_curPageNum_Wrap++;
	}

	return DM_ECODE_OK;
}

DMCode CSceneChoose::OnClose()
{
	if (m_pMainWnd && m_pMainWnd->IsWindow())
	{
		m_pMainWnd->OnClose();
	}

	return DM_ECODE_OK;
}

CSceneChoose::CSceneChoose(CMainWnd *pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_curPageNum_Wrap = 0;
}

// 显示WrapLayout中的图片
void CSceneChoose::ShowImageOfWrap(int sindex)
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
void CSceneChoose::ClearImageOfWrap(void)
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

//void CSceneChoose::ShowImage(const wchar_t *pFilePath, const wchar_t *lpszName, LPCWSTR lpszSkin)
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
void CSceneChoose::GetFilePathOfFmt(std::wstring dirpath, std::vector<std::wstring> &files, std::wstring fmt)
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

