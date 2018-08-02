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

	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnLButtonDbClick(UINT nFlags, CPoint pt);
	void OnLButtonUP(UINT nFlags, CPoint pt);
	bool OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	//void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
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
	HDMTREEITEM					  m_hSelItem_tree;		// 保存选取的tree项
	ImagePreview				  *p_SelImage;			// 当前选中的图片控件

private:
	
	DUIWrapLayout				  *pWrapLayout;
	DUIListBoxEx				  *pListBoxEx;
	DMSmartPtrT<CMainWnd>         m_pMainWnd;			// 主窗口
	CSceneShoot					  *pSceneShoot;			// 场景拍摄对象

	enum WNDTYPE
	{
		SCENE_CHOOSE = 0,
		SCENE_DETAIL = 1,
		SCENE_SHOOT = 2,
		SCENE_UNDEF
	};

	static const int PICHEIGHT_ONE = 200;				// 每张图片的高度
	static const int PICHEIGHT_AREA = 600;				// 图片区域的高度
	std::vector<ImagePreview *> m_vecChildPtr_Wrap;		// 保存WrapLayout中所有ImagePreview子控件的地址
	std::vector<DUIWindow *> m_vecWndPtr;				// 保存所有的窗口的地址
	wchar_t m_wcPicRootDir[MAX_PATH];					// 图片根目录

	bool m_strawcolor;									// 吸管是否开启

	// 图片信息 <图片目录路径名，该路径下所有文件路径名>
	std::map<CStringW, std::shared_ptr<std::set<std::wstring>>> m_mapPicInfo;
};


