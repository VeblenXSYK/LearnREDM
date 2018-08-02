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
	MSG_WM_LBUTTONUP(OnLButtonUP)
	MSG_WM_SETCURSOR(OnSetCursor)
	CHAIN_MSG_MAP(DMHWnd)// ��δ�������Ϣ����DMHWnd����
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
	EVENT_NAME_COMMAND(L"sceneshoot_strawbtn", OnSceneShootStraw)
	EVENT_NAME_COMMAND(L"sceneshoot_forebtn", OnForeground)
	EVENT_NAME_COMMAND(L"sceneshoot_exportbtn", OnExport)
	EVENT_NAME_COMMAND(L"sceneshoot_importbtn", OnImport)
	EVENT_NAME_COMMAND(L"prevpagebtn", OnPrepage)
	EVENT_NAME_COMMAND(L"nextpagebtn", OnNextpage)
END_EVENT_MAP()


BOOL CShootSystem::OnInitDialog(HWND wndFocus, LPARAM lInitParam)
{
	// ��ȡ���ڵ�ַ������
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"scenechoosewin"));
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"scenedetailwin"));
	m_vecWndPtr.push_back(FindChildByNameT<DUIWindow>(L"sceneshootwin"));

	// ��ȡ�ؼ�����
	m_pTreeCtrl = FindChildByNameT<DUITreeCtrl>(L"scenechoose_tree");
	pWrapLayout = FindChildByNameT<DUIWrapLayout>(L"scenechoose_wrap", TRUE);
	pListBoxEx = FindChildByNameT<DUIListBoxEx>(L"scenechoose_listboxex");

	GetRootFullPath(L".\\WorldTripShootRes\\outimage\\", m_wcPicRootDir, MAX_PATH);

	// ��ʼ����ʾ����ͯ-�����ھ�-ŷʽ��Լ��ͼƬ
	CStringW key = L"��ͯ\\�����ھ�\\ŷʽ��Լ";
	std::shared_ptr<std::set<std::wstring>> pSet(new std::set<std::wstring>);
	m_mapPicInfo[key] = pSet;
	CStringW strPath = m_wcPicRootDir + key;
	GetFilePathOfFmt(strPath, *m_mapPicInfo[key], L"png");
	ShowImageOfWrap(*m_mapPicInfo[key]);

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

void CShootSystem::OnLButtonUP(UINT nFlags, CPoint pt)
{
	if (m_strawcolor)
	{
		m_strawcolor = false;

		//// �õ���굱ǰλ�õ���ɫ
		//CPoint point;
		//GetCursorPos(&point);
		//HDC hDC = ::GetDC(NULL);
		//COLORREF RGBcolor = ::GetPixel(hDC, point.x, point.y);
		//::ReleaseDC(NULL, hDC);

		//if (RGBcolor != RGB(255, 255, 255))
		//{
		//	pSceneShoot->ShowStaticColor(RGB(255, 255, 255));
		//}
		//else
		//{
		//	pSceneShoot->ShowStaticColor(RGB(151, 103, 61));
		//}

		// �õ���굱ǰλ�õ���ɫ
		DMAutoDC hDC;

		CPoint point;
		GetCursorPos(&point);

		COLORREF RGBcolor = ::GetPixel(hDC, point.x, point.y);
		pSceneShoot->ShowStaticColor(RGBcolor);
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

DMCode CShootSystem::OnTreeSelChanged(DMEventArgs *pEvt)
{
	DMEventTCSelChangedArgs *pSelEvt = (DMEventTCSelChangedArgs*)pEvt;
	HDMTREEITEM hSelItem = pSelEvt->m_hNewSel;
	if (hSelItem)
	{
		// ����ѡ�е���
		m_hSelItem_tree = hSelItem;

		int iChildNum = m_pTreeCtrl->GetChildrenCount(hSelItem);

		// ������˵��������Ҫ��
		if(iChildNum > 0)
			return DM_ECODE_FAIL;

		// û�и���˵��������Ҫ��
		HDMTREEITEM hPSelItem = m_pTreeCtrl->GetParentItem(hSelItem);
		if(hPSelItem == NULL)
			return DM_ECODE_FAIL;

		ClearImageOfWrap();

		// ��ȡѡ������ļ���
		CStringW Seltext = m_pTreeCtrl->GetItemText(hSelItem);
		// ��ȡѡ����ĸ����ļ���
		CStringW PSeltext = m_pTreeCtrl->GetItemText(hPSelItem);

		// ��ȡ��ȫ��ѡ����
		CStringW total_Seltext;
		HDMTREEITEM hPPSelItem = m_pTreeCtrl->GetParentItem(hPSelItem);
		if (hPPSelItem != NULL)
		{
			// ��ȡѡ����ĸ�����ļ���
			CStringW PPSeltext = m_pTreeCtrl->GetItemText(hPPSelItem);
			total_Seltext = PPSeltext + L"\\" + PSeltext + L"\\" + Seltext;
		}
		else
			total_Seltext = PSeltext + L"\\" + Seltext;

		// ȷ���Ƿ��Ѿ������
		if (m_mapPicInfo.find(total_Seltext) == m_mapPicInfo.end())
		{
			// û�����̬����set
			std::shared_ptr<std::set<std::wstring>> pSet(new std::set<std::wstring>);
			m_mapPicInfo[total_Seltext] = pSet;

			if (PSeltext != L"Ԥѡ")
			{
				// ��ȡ������Դ��·��
				CStringW strPath = m_wcPicRootDir + total_Seltext;
				GetFilePathOfFmt(strPath, *m_mapPicInfo[total_Seltext], L"png");
			}
		}

		// ��ʾͼƬ
		ShowImageOfWrap(*(m_mapPicInfo[total_Seltext]));
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
		// ����Ԥѡ���ڣ�ģʽ�Ի���
		DMSmartPtrT<CPreChoose> pDlg; pDlg.Attach(new CPreChoose(this));
		int stat = pDlg->DoModal(L"prechoose", this->m_hWnd, true);
		if (stat == IDOK)
		{
			// ����Ԥѡ��Ϣ
			CStringW key = L"Ԥѡ\\" + pDlg->m_preChooseName;
			if (m_mapPicInfo.find(key) == m_mapPicInfo.end())
			{
				// û�ҵ���̬����set
				std::shared_ptr<std::set<std::wstring>> pSet(new std::set<std::wstring>);
				m_mapPicInfo[key] = pSet;
			}
			m_mapPicInfo[key]->insert(this->p_SelImage->m_picPath);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode CShootSystem::OnDelPreChoose()
{
	if (m_hSelItem_tree != NULL)
	{
		// û�и���˵��������Ҫ��
		HDMTREEITEM hPSelItem = m_pTreeCtrl->GetParentItem(m_hSelItem_tree);
		if (hPSelItem == NULL)
			return DM_ECODE_FAIL;

		// û��ѡ��˵��������Ҫ��
		if (p_SelImage == NULL)
			return DM_ECODE_FAIL;

		// ��ȡѡ����ĸ����ļ���
		CStringW PSeltext = m_pTreeCtrl->GetItemText(hPSelItem);

		// ֻ�и���Ϊ��Ԥѡ��������ܽ���Ԥѡɾ��
		if (PSeltext == L"Ԥѡ")
		{
			// ��ȡѡ������ļ���
			CStringW Seltext = m_pTreeCtrl->GetItemText(m_hSelItem_tree);
			CStringW keymap = PSeltext + L"\\" + Seltext;

			// ����ѡ�е�ͼƬ�ؼ�
			std::set<std::wstring> &selset = *m_mapPicInfo[keymap];
			std::set<std::wstring>::iterator it = selset.begin();
			for (; it != selset.end(); it++)
			{
				if (*it == p_SelImage->m_picPath)
				{
					selset.erase(it);

					// ������ʾͼƬ
					ClearImageOfWrap();
					ShowImageOfWrap(selset);

					break;
				}
			}

			// m_pTreeCtrl->RemoveItem(m_hSelItem_tree);
			// m_hSelItem_tree = NULL;
		}
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnOneShootChoose()
{
	// �رա�ѡ�񳡾�������
	m_vecWndPtr[SCENE_CHOOSE]->DM_SetVisible(FALSE);

	// ��ʾ���������顱����
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
		// ����Ԥѡ���ڣ�ģʽ�Ի���
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
	// �رա��������顱����
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(FALSE);

	// ��ʾ��ѡ�񳡾�������
	m_vecWndPtr[SCENE_CHOOSE]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneDetailShoot()
{
	// �رա��������顱����
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(FALSE);

	// ��ʾ���������㡱����
	m_vecWndPtr[SCENE_SHOOT]->DM_SetVisible(TRUE, TRUE);

	// ��ʼ�����������㡱
	pSceneShoot->Init();

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootReturn()
{
	// �رա��������㡱����
	m_vecWndPtr[SCENE_SHOOT]->DM_SetVisible(FALSE);

	// ��ʾ���������顱����
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootStraw()
{
	m_strawcolor = true;

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
	m_hSelItem_tree = NULL;
	p_SelImage = NULL;
	m_strawcolor = false;
	pSceneShoot = new CSceneShoot(this);
}

// ��ʾWrapLayout�е�ͼƬ
void CShootSystem::ShowImageOfWrap(std::set<std::wstring> &filepaths)
{
	std::set<std::wstring>::iterator it = filepaths.begin();
	for (; it != filepaths.end(); it++)
	{
		const wchar_t *szFilePath = (*it).c_str();
		size_t ulSize = DM::GetFileSizeW(szFilePath);

		DWORD dwReadSize = 0;
		DMBufT<byte>pBuf; pBuf.Allocate(ulSize);
		DM::GetFileBufW(szFilePath, (void **)&pBuf, ulSize, dwReadSize);

		// ����Ԥ��skin
		DMSmartPtrT<IDMSkin> pSkin;
		g_pDMApp->CreateRegObj((void**)&pSkin, L"imglist", DMREG_Skin);
		pSkin->SetBitmap(pBuf, ulSize, L"png");

		// ����ImagePreview�ӿؼ�
		ImagePreview *pChild = NULL;
		g_pDMApp->CreateRegObj((void**)&pChild, L"ImagePreview", DMREG_Window);
		if (pChild)
		{
			pChild->m_pSkin = pSkin;
			pChild->m_picPath = *it;					// ����ͼƬ��·��
			pChild->m_pShootSystem = this;				// ����ShootSystem����ĵ�ַ
			pWrapLayout->DM_InsertChild(pChild);
			m_vecChildPtr_Wrap.push_back(pChild);
		}
	}

	// ������Ҫ����ListBoxEx�ĸ߶�
	int filenum = filepaths.size();
	if (ceil(filenum / 5.0) * PICHEIGHT_ONE > PICHEIGHT_AREA)
		pListBoxEx->SetItemHeight(0, ceil(filenum / 5.0) * PICHEIGHT_ONE, true);

	pWrapLayout->DV_UpdateChildLayout();
}

// ���WrapLayout�е�ͼƬ
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

		p_SelImage = NULL;
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
//		{// ���skin���ڣ���ֱ��ɾ��
//#if 0// ���skin���ڣ���ֱ�Ӹ��ڴ�
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
//		// ���skin�����ڣ��򴴽�skin
//		CStringW strWXml;
//		strWXml.Format(L"<imglist id=\"%s\" states=\"3\"/>", lpszSkin);								// xml�в�Ҫ��src��		
//		g_pDMApp->AddSkin(pBuf, dwSize, L"png", strWXml, L"ccskinpool");							// ���뵽ccskinpool���skin��
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

//// ��ȡָ��Ŀ¼���ض���ʽ�������ļ�·��
//void CShootSystem::GetFilePathOfFmt(std::wstring dirpath, std::set<std::wstring> &filepaths, std::wstring fmt)
//{
//	//�ļ����
//	long hFile = 0;
//
//	//�ļ���Ϣ
//	struct _wfinddata_t fileinfo;
//	std::wstring p;
//	if ((hFile = _wfindfirst(p.assign(dirpath).append(L"\\*" + fmt).c_str(), &fileinfo)) != -1)
//	{
//		do
//		{
//			filepaths.insert(p.assign(dirpath).append(L"\\").append(fileinfo.name));
//		} while (_wfindnext(hFile, &fileinfo) == 0);
//		_findclose(hFile);
//	}
//}

// ��ȡָ��Ŀ¼���ض���ʽ�������ļ�·��
void CShootSystem::GetFilePathOfFmt(CStringW dirpath, std::set<std::wstring> &filepaths, std::wstring fmt)
{
	//�ļ����
	long hFile = 0;

	//�ļ���Ϣ
	struct _wfinddata_t fileinfo;
	std::wstring p;
	if ((hFile = _wfindfirst(p.assign(dirpath).append(L"\\*" + fmt).c_str(), &fileinfo)) != -1)
	{
		do
		{
			filepaths.insert(p.assign(dirpath).append(L"\\").append(fileinfo.name));
		} while (_wfindnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

