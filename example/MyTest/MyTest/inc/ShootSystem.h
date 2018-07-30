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

	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnLButtonDbClick(UINT nFlags, CPoint pt);
	//void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
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
	DMSmartPtrT<CMainWnd>         m_pMainWnd;			// 主窗口
	DUIWindow					  *pWndTitle;			// 窗口标题编辑框
	DUIWindow					  *pSceneChooseWnd;		// 场景选择窗口
	DUIWindow					  *pSceneDetailWnd;		// 场景详情窗口
	DUIWindow					  *pSceneShootWnd;		// 场景拍摄窗口
	CSceneShoot					  *pSceneShoot;			// 场景拍摄对象

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

	HDMTREEITEM m_hSelItem_tree;						// 保存选取的tree项

	static const int PICHEIGHT_ONE = 200;				// 每张图片的高度
	static const int PICHEIGHT_AREA = 600;				// 图片区域的高度
	std::vector<ImagePreview *> m_vecChildPtr_Wrap;		// 保存WrapLayout中所有ImagePreview子控件的地址
	std::vector<DUIWindow *> m_vecWndPtr;				// 保存所有的窗口的地址
	std::vector<std::wstring> m_vecWndTitle;			// 保存所有的窗口的标题
	int m_curPageNum_Wrap;								// WrapLayout中的当前显示页
	int m_curShowPicType;								// 当前显示的图片类型
	int m_curWndType;									// 当前窗口类型

	wchar_t m_wcPicDirPath_European[MAX_PATH];			// 欧式简约图片目录路径
	wchar_t m_wcPicDirPath_Pastoral[MAX_PATH];			// 唯美田园图片目录路径
	std::vector<std::wstring> m_vecPicPath_European;	// 保存欧式简约所有图片的路径
	std::vector<std::wstring> m_vecPicPath_Pastoral;	// 保存唯美田园所有图片的路径
};


