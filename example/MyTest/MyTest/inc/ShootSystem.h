#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>


class CMainWnd;
class ImagePreview;
class CPreChoose;
class CSceneShoot;
class CShootSystem : public DMHWnd
{
public:
	CShootSystem::CShootSystem(CMainWnd *);
	void ShowImageOfWrap(std::set<std::wstring> &);
	void ClearImageOfWrap(void);
	void GetFilePathOfFmt(CStringW dirpath, std::set<std::wstring> &, std::wstring);

	DECLARE_MESSAGE_MAP()// ��MFC��Ϣӳ���
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: ��Ϣ�ַ�ϵ�к���
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnLButtonDbClick(UINT nFlags, CPoint pt);
	void OnLButtonUP(UINT nFlags, CPoint pt);
	bool OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	//void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: �¼��ַ�ϵ�к���
	//---------------------------------------------------
	DMCode OnTreeSelChanged(DMEventArgs *pEvt);
	DMCode OnLightSDChanged(DMEventArgs *pEvt);
	DMCode OnSceneChooseReturn();
	DMCode OnAddPreChoose();
	DMCode OnDelPreChoose();
	DMCode OnOneShootChoose();
	DMCode OnForeground();
	DMCode OnExport();
	DMCode OnImport();
	DMCode OnSceneDetailReturn();
	DMCode OnSceneDetailShoot();
	DMCode OnSceneShootReturn();
	DMCode OnSceneShootStraw();
	DMCode OnPrepage();
	DMCode OnNextpage();
	DMCode OnClose();

	DUITreeCtrl					  *m_pTreeCtrl;
	HDMTREEITEM					  m_hSelItem_tree;		// ����ѡȡ��tree��
	ImagePreview				  *p_SelImage;			// ��ǰѡ�е�ͼƬ�ؼ�

private:
	
	DUIWrapLayout				  *pWrapLayout;
	DUIListBoxEx				  *pListBoxEx;
	DMSmartPtrT<CMainWnd>         m_pMainWnd;			// ������
	CSceneShoot					  *pSceneShoot;			// �����������

	enum WNDTYPE
	{
		SCENE_CHOOSE = 0,
		SCENE_DETAIL = 1,
		SCENE_SHOOT = 2,
		SCENE_UNDEF
	};

	static const int PICHEIGHT_ONE = 200;				// ÿ��ͼƬ�ĸ߶�
	static const int PICHEIGHT_AREA = 600;				// ͼƬ����ĸ߶�
	std::vector<ImagePreview *> m_vecChildPtr_Wrap;		// ����WrapLayout������ImagePreview�ӿؼ��ĵ�ַ
	std::vector<DUIWindow *> m_vecWndPtr;				// �������еĴ��ڵĵ�ַ
	wchar_t m_wcPicRootDir[MAX_PATH];					// ͼƬ��Ŀ¼

	bool m_strawcolor;									// �����Ƿ���

	// ͼƬ��Ϣ <ͼƬĿ¼·��������·���������ļ�·����>
	std::map<CStringW, std::shared_ptr<std::set<std::wstring>>> m_mapPicInfo;
};


