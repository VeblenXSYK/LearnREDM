#include "StdAfx.h"
#include "MainWnd.h"
#include "ImagePreview.h"
#include "PreChoose.h"
#include "ExportSet.h"
#include "SceneShoot.h"
#include "ShootSystem.h"
#include "CommModule.h"

#include <direct.h>
#include <io.h>
#include <fstream>
#include <atlconv.h>

BEGIN_MSG_MAP(CShootSystem)
	MSG_WM_INITDIALOG(OnInitDialog)
	MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
	MSG_WM_LBUTTONUP(OnLButtonUP)
	MSG_WM_SETCURSOR(OnSetCursor)
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
	EVENT_NAME_COMMAND(L"scenechoose_allbtn", OnAllShootChoose)
	EVENT_NAME_COMMAND(L"scenedetail_returnbtn", OnSceneDetailReturn)
	EVENT_NAME_COMMAND(L"scenedetail_shootbtn", OnSceneDetailShoot)
	EVENT_NAME_COMMAND(L"sceneshoot_returnbtn", OnSceneShootReturn)
	EVENT_NAME_COMMAND(L"sceneshoot_rotatebtn", OnSceneShootRotate)
	EVENT_NAME_COMMAND(L"sceneshoot_strawbtn", OnSceneShootStraw)
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
	pWrapLayout = FindChildByNameT<DUIWrapLayout>(L"scenechoose_wrap", TRUE);
	pListBoxEx = FindChildByNameT<DUIListBoxEx>(L"scenechoose_listboxex");

	// 初始化显示“儿童-精美内景-欧式简约”图片
	CStringW key = L"儿童\\精美内景\\欧式简约";
	std::shared_ptr<std::set<std::wstring>> pSet(new std::set<std::wstring>);
	m_mapPicInfo[key] = pSet;
	CStringW strPath = CCommModule::GetPicRootDir() + key;
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

		//// 得到鼠标当前位置的颜色
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

		// 得到鼠标当前位置的颜色
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
		// 保存选中的项
		m_hSelItem_tree = hSelItem;

		int iChildNum = m_pTreeCtrl->GetChildrenCount(hSelItem);

		// 有子项说明不符合要求
		if(iChildNum > 0)
			return DM_ECODE_FAIL;

		// 没有父项说明不符合要求
		HDMTREEITEM hPSelItem = m_pTreeCtrl->GetParentItem(hSelItem);
		if(hPSelItem == NULL)
			return DM_ECODE_FAIL;

		ClearImageOfWrap();

		// 获取选中项的文件名
		CStringW Seltext = m_pTreeCtrl->GetItemText(hSelItem);
		// 获取选中项的父项文件名
		CStringW PSeltext = m_pTreeCtrl->GetItemText(hPSelItem);

		// 获取完全的选择名
		CStringW total_Seltext;
		HDMTREEITEM hPPSelItem = m_pTreeCtrl->GetParentItem(hPSelItem);
		if (hPPSelItem != NULL)
		{
			// 获取选中项的父项父项文件名
			CStringW PPSeltext = m_pTreeCtrl->GetItemText(hPPSelItem);
			total_Seltext = PPSeltext + L"\\" + PSeltext + L"\\" + Seltext;
		}
		else
			total_Seltext = PSeltext + L"\\" + Seltext;

		// 确认是否已经点击过
		if (m_mapPicInfo.find(total_Seltext) == m_mapPicInfo.end())
		{
			// 没点击则动态创建set
			std::shared_ptr<std::set<std::wstring>> pSet(new std::set<std::wstring>);
			m_mapPicInfo[total_Seltext] = pSet;

			if (PSeltext != L"预选")
			{
				// 获取查找资源的路径
				CStringW strPath = CCommModule::GetPicRootDir() + total_Seltext;
				GetFilePathOfFmt(strPath, *m_mapPicInfo[total_Seltext], L"psd");
			}
		}

		// 显示图片
		ShowImageOfWrap(*(m_mapPicInfo[total_Seltext]), 1);
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
		int stat = pDlg->DoModal(L"prechoose", this->m_hWnd, true);
		if (stat == IDOK)
		{
			// 插入预选信息
			CStringW key = L"预选\\" + pDlg->m_preChooseName;
			if (m_mapPicInfo.find(key) == m_mapPicInfo.end())
			{
				// 没找到则动态创建set
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
		// 没有父项说明不符合要求
		HDMTREEITEM hPSelItem = m_pTreeCtrl->GetParentItem(m_hSelItem_tree);
		if (hPSelItem == NULL)
			return DM_ECODE_FAIL;

		// 没有选中说明不符合要求
		if (p_SelImage == NULL)
			return DM_ECODE_FAIL;

		// 获取选中项的父项文件名
		CStringW PSeltext = m_pTreeCtrl->GetItemText(hPSelItem);

		// 只有父项为“预选”的项才能进行预选删除
		if (PSeltext == L"预选")
		{
			// 获取选中项的文件名
			CStringW Seltext = m_pTreeCtrl->GetItemText(m_hSelItem_tree);
			CStringW keymap = PSeltext + L"\\" + Seltext;

			// 擦除选中的图片控件
			std::set<std::wstring> &selset = *m_mapPicInfo[keymap];
			std::set<std::wstring>::iterator it = selset.begin();
			for (; it != selset.end(); it++)
			{
				if (*it == p_SelImage->m_picPath)
				{
					selset.erase(it);

					// 重新显示图片
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
	m_shootmode = ONE_CHOOSE;

	if (p_SelImage == NULL)
	{
		return DM_ECODE_FAIL;
	}

	// 将该选择场景路径传给dll中处理
	LOG_USER("%s\n", p_SelImage->m_picPath.c_str());
	
	// 关闭“选择场景”窗口
	m_vecWndPtr[SCENE_CHOOSE]->DM_SetVisible(FALSE);

	// 显示“场景详情”窗口
	m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(TRUE, TRUE);

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnAllShootChoose()
{
	// 确保点击TreeCtrl
	if(m_hSelItem_tree == NULL)
		return DM_ECODE_FAIL;

	// 没有父项说明不符合要求
	HDMTREEITEM hPSelItem = m_pTreeCtrl->GetParentItem(m_hSelItem_tree);
	if (hPSelItem == NULL)
		return DM_ECODE_FAIL;

	// 获取选中项的父项文件名
	CStringW PSeltext = m_pTreeCtrl->GetItemText(hPSelItem);
	// 只有父项为“预选”的项才能进行“全选拍摄”
	if (PSeltext == L"预选")
	{
		// 获取选中项的文件名
		CStringW Seltext = m_pTreeCtrl->GetItemText(m_hSelItem_tree);
		CStringW keymap = PSeltext + L"\\" + Seltext;

		// 从所有选取的路径名中取第一个给dll处理
		m_pPathAllShoot = m_mapPicInfo[keymap];
		if (m_pPathAllShoot->size() != 0)
		{
			m_ItorAllShoot = m_pPathAllShoot->begin();
			LOG_USER("%s\n", m_ItorAllShoot->c_str());

			// 关闭“选择场景”窗口
			m_vecWndPtr[SCENE_CHOOSE]->DM_SetVisible(FALSE);

			// 显示“场景详情”窗口
			m_vecWndPtr[SCENE_DETAIL]->DM_SetVisible(TRUE, TRUE);

			m_shootmode = ALL_CHOOSE;
		}
	}

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

DMCode CShootSystem::OnSceneShootStraw()
{
	m_strawcolor = true;

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnSceneShootRotate()
{
	pSceneShoot->HandleRotate();
	return DM_ECODE_OK;
}

DMCode CShootSystem::OnPrepage()
{
	if (m_shootmode == ALL_CHOOSE)
	{
		if (m_ItorAllShoot != m_pPathAllShoot->begin())
		{
			m_ItorAllShoot--;
		}
		else
		{
			// 指向最后一个元素
			m_ItorAllShoot = --(m_pPathAllShoot->end());
		}
		LOG_USER("%s", m_ItorAllShoot->c_str());
	}

	return DM_ECODE_OK;
}

DMCode CShootSystem::OnNextpage()
{
	if (m_shootmode == ALL_CHOOSE)
	{
		m_ItorAllShoot++;
		if (m_ItorAllShoot == m_pPathAllShoot->end())
		{
			m_ItorAllShoot = m_pPathAllShoot->begin();
		}
		LOG_USER("%s", m_ItorAllShoot->c_str());
	}
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
	m_shootmode = UNDEF_CHOOSE;
	m_hSelItem_tree = NULL;
	p_SelImage = NULL;
	m_strawcolor = false;
	pSceneShoot = new CSceneShoot(this);
}

// 显示WrapLayout中的图片
void CShootSystem::ShowImageOfWrap(std::set<std::wstring> &filepaths, int default)
{
	std::set<std::wstring>::iterator it = filepaths.begin();
	for (; it != filepaths.end(); it++)
	{
		const wchar_t *szFilePath = (*it).c_str();
		
		size_t ulSize;
		DMBufT<byte>pBuf;
		if (default == 0)
		{
			ulSize = DM::GetFileSizeW(szFilePath);
			DWORD dwReadSize = 0;
			pBuf.Allocate(ulSize);
			DM::GetFileBufW(szFilePath, (void **)&pBuf, ulSize, dwReadSize);
		}
		else
		{
			USES_CONVERSION;
			// 加载psd文件
			PsdParser parser(W2A(szFilePath));
			// 生成缩略图
			ulSize = parser.thumbnail((void **)&pBuf);
		}

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
			pChild->m_picPath = *it;					// 保存图片的路径
			pChild->m_pShootSystem = this;				// 保存ShootSystem对象的地址
			pWrapLayout->DM_InsertChild(pChild);
			m_vecChildPtr_Wrap.push_back(pChild);
		}
	}

	// 根据需要调整ListBoxEx的高度
	int filenum = filepaths.size();
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

//// 获取指定目录下特定格式的所有文件路径
//void CShootSystem::GetFilePathOfFmt(std::wstring dirpath, std::set<std::wstring> &filepaths, std::wstring fmt)
//{
//	//文件句柄
//	long hFile = 0;
//
//	//文件信息
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

// 获取指定目录下特定格式的所有文件路径
void CShootSystem::GetFilePathOfFmt(CStringW dirpath, std::set<std::wstring> &filepaths, std::wstring fmt)
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
			filepaths.insert(p.assign(dirpath).append(L"\\").append(fileinfo.name));
		} while (_wfindnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

