#pragma once

#include <vector>
#include <string>

class CMainWnd;
class ImagePreview;
class CPreChoose;
class CSceneShoot;
class CShootSystem : public DMHWnd
{
public:
	CShootSystem::CShootSystem(CMainWnd *);
	void ShowImageOfWrap();
	void ClearImageOfWrap(void);
	void GetFilePathOfFmt(std::wstring dirpath, std::vector<std::wstring> &files, std::wstring fmt);

	DECLARE_MESSAGE_MAP()// ��MFC��Ϣӳ���
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: ��Ϣ�ַ�ϵ�к���
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnLButtonDbClick(UINT nFlags, CPoint pt);
	//void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: �¼��ַ�ϵ�к���
	//---------------------------------------------------
	DMCode OnTreeSelChanged(DMEventArgs *pEvt);
	DMCode OnLightSDChanged(DMEventArgs *pEvt);
	DMCode OnAddPreChoose();
	DMCode OnDelPreChoose();
	DMCode OnForeground();
	DMCode OnExport();
	DMCode OnImport();
	DMCode OnReturn();
	DMCode OnPrepage();
	DMCode OnNextpage();
	DMCode OnClose();

	DUITreeCtrl					  *m_pTreeCtrl;

private:
	DUIWindow					  *pWrapLayout;
	DUIListBoxEx				  *pListBoxEx;
	DMSmartPtrT<CMainWnd>         m_pMainWnd;			// ������
	DUIWindow					  *pWndTitle;			// ���ڱ���༭��
	DUIWindow					  *pSceneChooseWnd;		// ����ѡ�񴰿�
	DUIWindow					  *pSceneDetailWnd;		// �������鴰��
	DUIWindow					  *pSceneShootWnd;		// �������㴰��
	CSceneShoot					  *pSceneShoot;			// �����������

	enum WNDTYPE
	{
		SCENE_CHOOSE = 0,
		SCENE_DETAIL = 1,
		SCENE_SHOOT = 2,
		SCENE_UNDEF
	};
	wchar_t m_WndTitle[SCENE_UNDEF];

	enum PICTYPE
	{
		EUROPEAN,
		PASTORAL
	};

	HDMTREEITEM m_hSelItem_tree;						// ����ѡȡ��tree��

	static const int PICHEIGHT_ONE = 200;				// ÿ��ͼƬ�ĸ߶�
	static const int PICHEIGHT_AREA = 600;				// ͼƬ����ĸ߶�
	std::vector<ImagePreview *> m_vecChildPtr_Wrap;		// ����WrapLayout������ImagePreview�ӿؼ��ĵ�ַ
	std::vector<DUIWindow *> m_vecWndPtr;				// �������еĴ��ڵĵ�ַ
	std::vector<std::wstring> m_vecWndTitle;			// �������еĴ��ڵı���
	int m_curPageNum_Wrap;								// WrapLayout�еĵ�ǰ��ʾҳ
	int m_curShowPicType;								// ��ǰ��ʾ��ͼƬ����
	int m_curWndType;									// ��ǰ��������

	wchar_t m_wcPicDirPath_European[MAX_PATH];			// ŷʽ��ԼͼƬĿ¼·��
	wchar_t m_wcPicDirPath_Pastoral[MAX_PATH];			// Ψ����԰ͼƬĿ¼·��
	std::vector<std::wstring> m_vecPicPath_European;	// ����ŷʽ��Լ����ͼƬ��·��
	std::vector<std::wstring> m_vecPicPath_Pastoral;	// ����Ψ����԰����ͼƬ��·��
};


