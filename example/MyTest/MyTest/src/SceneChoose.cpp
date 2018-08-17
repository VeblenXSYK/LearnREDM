#include "StdAfx.h"
#include "CommModule.h"
#include "ImagePreview.h"
#include "SceneShoot.h"
#include "ShootSystem.h"
#include "PreChoose.h"
#include "SceneChoose.h"

#include <direct.h>
#include <io.h>
#include <fstream>
#include <atlconv.h>

CSceneChoose::CSceneChoose(CShootSystem *pShootSystem)
{
	m_shootmode = UNDEF_CHOOSE;
	m_hSelItem_tree = 0;
	m_pSelImage = NULL;
	m_pShootSystem = pShootSystem;
}

void CSceneChoose::HandleParsePsdMessage(std::wstring wpath, int messagetype)
{
	switch (messagetype)
	{
		case CGuiMessage::SECENECHOOSE_PARSEPSD_ONE:
		case CGuiMessage::SECENECHOOSE_PARSEPSD_ALL:
			// 打开Psd文件
			OpenPsdFile(wpath);
			// 获取背景图片
			GetBackgroundLayerImageData();
			// 获取前景图片
			GetForegroundLayerImageData();
			// 改变“场景详情”背景
			m_pShootSystem->ChangeSceneDetailBg(m_curBgImage);
			// 关闭“选择场景”窗口
			m_pShootSystem->m_vecWndPtr[CShootSystem::SCENE_CHOOSE]->DM_SetVisible(FALSE);
			// 显示“场景详情”窗口
			m_pShootSystem->m_vecWndPtr[CShootSystem::SCENE_DETAIL]->DM_SetVisible(TRUE, TRUE);
			break;
		case CGuiMessage::SECENEDETAIL_PARSEPSD_PREV:
		case CGuiMessage::SECENEDETAIL_PARSEPSD_NEXT:
			// 打开Psd文件
			OpenPsdFile(wpath);
			// 获取背景图片
			GetBackgroundLayerImageData();
			// 获取前景图片
			GetForegroundLayerImageData();
			// 改变“场景详情”背景
			m_pShootSystem->ChangeSceneDetailBg(m_curBgImage);
			break;
		case CGuiMessage::SECENEDETAIL_PARSEPSD_SHOOT:
			// 更换场景拍摄背景图片
			m_pShootSystem->m_pSceneShoot->ChangeSceneShootBg(m_curBgImage);
			// 更换场景拍摄前景图片
			m_pShootSystem->m_pSceneShoot->ChangeSceneShootFg(m_curFgImage);
			// 关闭“场景详情”窗口
			m_pShootSystem->m_vecWndPtr[CShootSystem::SCENE_DETAIL]->DM_SetVisible(FALSE);
			// 显示“场景拍摄”窗口
			m_pShootSystem->m_vecWndPtr[CShootSystem::SCENE_SHOOT]->DM_SetVisible(TRUE, TRUE);
			// 初始化“场景拍摄”
			m_pShootSystem->m_pSceneShoot->Init();
			break;
		case CGuiMessage::SECENESHOOT_PARSEPSD_PREV:
		case CGuiMessage::SECENESHOOT_PARSEPSD_NEXT:
			// 打开Psd文件
			OpenPsdFile(wpath);
			// 获取背景图片
			GetBackgroundLayerImageData();
			// 获取前景图片
			GetForegroundLayerImageData();
			// 更换场景拍摄背景图片
			m_pShootSystem->m_pSceneShoot->ChangeSceneShootBg(m_curBgImage);
			// 更换场景拍摄前景图片
			m_pShootSystem->m_pSceneShoot->ChangeSceneShootFg(m_curFgImage);
			break;
		default:
			break;
	}
}

void CSceneChoose::ProduceParsePsdMessage(std::wstring &path, int message)
{
	// 保存当前路径
	m_curImagepath = path;

	// 创建消息对象
	CGuiMessage guiMessage;
	// 设置消息回调函数
	guiMessage.SetMessageCallback(std::bind(&CSceneChoose::HandleParsePsdMessage, this, path, message));
	// 加入消息队列进行处理
	g_MessageTaskQueue.Push(guiMessage);
}

void CSceneChoose::OpenPsdFile(std::wstring &wpath)
{
	// 打开Psd文件
	std::string outMsg;
	std::string path = CCommModule::GetRawString(CCommModule::WS2S(wpath));
	PSOpenFile(CCommModule::GetPSHandle(), path, outMsg);
}

void CSceneChoose::GetBackgroundLayerImageData(void)
{
	std::string outBgMsg;
	PSGetBackgroundLayerImage(CCommModule::GetPSHandle(), 1416, 797, outBgMsg);
	m_curBgImage = std::move(outBgMsg);
}

void CSceneChoose::GetForegroundLayerImageData(void)
{
	std::string outFgMsg;
	PSGetForegroundLayerImage(CCommModule::GetPSHandle(), 1416, 797, outFgMsg);
	m_curFgImage = std::move(outFgMsg);
}

void CSceneChoose::Init(void)
{
	// 获取控件名称
	m_pTreeCtrl = m_pShootSystem->FindChildByNameT<DUITreeCtrl>(L"scenechoose_tree");
	m_pWrapLayout = m_pShootSystem->FindChildByNameT<DUIWrapLayout>(L"scenechoose_wrap", TRUE);
	m_pListBoxEx = m_pShootSystem->FindChildByNameT<DUIListBoxEx>(L"scenechoose_listboxex");

	// 初始化显示“儿童-精美内景-欧式简约”图片
	CStringW key = L"儿童\\精美内景\\欧式简约";
	std::shared_ptr<std::set<std::wstring>> pSet(new std::set<std::wstring>);
	m_mapPicInfo[key] = pSet;
	CStringW strPath = CCommModule::GetPicRootDir() + key;
	GetFilePathOfFmt(strPath, *m_mapPicInfo[key], L"png");
	ShowImageOfWrap(*m_mapPicInfo[key]);
}

DMCode CSceneChoose::HandleTreeSelChanged(DMEventArgs *pEvt)
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

DMCode CSceneChoose::HandleAddPreChoose()
{
	DMCode iErr = DM_ECODE_FAIL;

	do
	{
		// 创建预选窗口（模式对话框）
		DMSmartPtrT<CPreChoose> pDlg; pDlg.Attach(new CPreChoose(this));
		int stat = pDlg->DoModal(L"prechoose", this->m_pShootSystem->m_hWnd, true);
		if (stat == IDOK)
		{
			// 插入预选信息
			CStringW key = L"预选\\" + pDlg->m_PreChooseName;
			if (m_mapPicInfo.find(key) == m_mapPicInfo.end())
			{
				// 没找到则动态创建set
				std::shared_ptr<std::set<std::wstring>> pSet(new std::set<std::wstring>);
				m_mapPicInfo[key] = pSet;
			}
			m_mapPicInfo[key]->insert(m_pSelImage->m_picPath);
		}

		iErr = DM_ECODE_OK;
	} while (false);
	return iErr;
}

DMCode CSceneChoose::HandleDelPreChoose()
{
	if (m_hSelItem_tree != NULL)
	{
		// 没有父项说明不符合要求
		HDMTREEITEM hPSelItem = m_pTreeCtrl->GetParentItem(m_hSelItem_tree);
		if (hPSelItem == NULL)
			return DM_ECODE_FAIL;

		// 没有选中说明不符合要求
		if (m_pSelImage == NULL)
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
				if (*it == m_pSelImage->m_picPath)
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

DMCode CSceneChoose::HandleOneShootChoose()
{
	m_shootmode = ONE_CHOOSE;

	if (m_pSelImage == NULL)
	{
		return DM_ECODE_FAIL;
	}

	ProduceParsePsdMessage(m_pSelImage->m_picPath, CGuiMessage::SECENECHOOSE_PARSEPSD_ONE);

	return DM_ECODE_OK;
}

DMCode CSceneChoose::HandleAllShootChoose()
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
			
			ProduceParsePsdMessage(static_cast<std::wstring>(*m_ItorAllShoot), CGuiMessage::SECENECHOOSE_PARSEPSD_ALL);

			m_shootmode = ALL_CHOOSE;
		}
	}

	return DM_ECODE_OK;
}

DMCode CSceneChoose::HandlePrevpage(int wndtype)
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

		if (wndtype == CShootSystem::SCENE_DETAIL)
			ProduceParsePsdMessage(static_cast<std::wstring>(*m_ItorAllShoot), CGuiMessage::SECENEDETAIL_PARSEPSD_PREV);
		else
			ProduceParsePsdMessage(static_cast<std::wstring>(*m_ItorAllShoot), CGuiMessage::SECENESHOOT_PARSEPSD_PREV);
	}

	return DM_ECODE_OK;
}

DMCode CSceneChoose::HandleNextpage(int wndtype)
{
	if (m_shootmode == ALL_CHOOSE)
	{
		m_ItorAllShoot++;
		if (m_ItorAllShoot == m_pPathAllShoot->end())
		{
			m_ItorAllShoot = m_pPathAllShoot->begin();
		}

		if(wndtype == CShootSystem::SCENE_DETAIL)
			ProduceParsePsdMessage(static_cast<std::wstring>(*m_ItorAllShoot), CGuiMessage::SECENEDETAIL_PARSEPSD_NEXT);
		else
			ProduceParsePsdMessage(static_cast<std::wstring>(*m_ItorAllShoot), CGuiMessage::SECENESHOOT_PARSEPSD_NEXT);
	}
	return DM_ECODE_OK;
}

// 显示WrapLayout中的图片
void CSceneChoose::ShowImageOfWrap(std::set<std::wstring> &filepaths, int default)
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

		// 创建CImagePreview子控件
		CImagePreview *pChild = NULL;
		g_pDMApp->CreateRegObj((void**)&pChild, L"ImagePreview", DMREG_Window);
		if (pChild)
		{
			pChild->m_pSkin = pSkin;
			pChild->m_picPath = *it;					// 保存图片的路径
			pChild->m_pSceneChoose = this;				// 保存SceneChoose对象的地址
			m_pWrapLayout->DM_InsertChild(pChild);
			m_vecChildPtr_Wrap.push_back(pChild);
		}
	}

	// 根据需要调整ListBoxEx的高度
	int filenum = filepaths.size();
	if (ceil(filenum / 5.0) * PICHEIGHT_ONE > PICHEIGHT_AREA)
		m_pListBoxEx->SetItemHeight(0, ceil(filenum / 5.0) * PICHEIGHT_ONE, true);

	m_pWrapLayout->DV_UpdateChildLayout();
}

// 清空WrapLayout中的图片
void CSceneChoose::ClearImageOfWrap(void)
{
	while (!m_vecChildPtr_Wrap.empty())
	{
		CImagePreview *pChild = m_vecChildPtr_Wrap.back();
		if (pChild != NULL)
		{
			m_pWrapLayout->DM_DestroyChildWnd(pChild);
			m_vecChildPtr_Wrap.pop_back();
		}

		m_pSelImage = NULL;
	}
}

// 获取指定目录下特定格式的所有文件路径
void CSceneChoose::GetFilePathOfFmt(CStringW dirpath, std::set<std::wstring> &filepaths, std::wstring fmt)
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
