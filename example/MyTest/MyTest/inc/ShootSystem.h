#pragma once

#include <vector>

class CMainWnd;
class CSceneShoot;
class CSceneChoose;
class CShootSystem : public DMHWnd
{
public:
	enum WNDTYPE
	{
		SCENE_CHOOSE = 0,
		SCENE_DETAIL = 1,
		SCENE_SHOOT = 2,
		SCENE_UNDEF
	};

	CShootSystem::CShootSystem(CMainWnd *);

	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);
	void OnLButtonDbClick(UINT nFlags, CPoint pt);
	void OnLButtonUP(UINT nFlags, CPoint pt);
	bool OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
	//void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnCloseBtn();
	DMCode OnSceneChooseTreeSelChanged(DMEventArgs *pEvt);
	DMCode OnSceneShootLightSDChanged(DMEventArgs *pEvt);
	DMCode OnSceneChooseReturnBtn();
	DMCode OnSceneChooseAddPreBtn();
	DMCode OnSceneChooseDelPreBtn();
	DMCode OnSceneChooseOneShootBtn();
	DMCode OnSceneChooseAllShootBtn();
	DMCode OnForegroundBtn();
	DMCode OnExportBtn();
	DMCode OnImportBtn();
	DMCode OnSceneDetailReturnBtn();
	DMCode OnSceneDetailShootBtn();
	DMCode OnSceneShootReturnBtn();
	DMCode OnSceneShootRotateBtn();
	DMCode OnSceneShootStrawBtn();
	DMCode OnPrepage();
	DMCode OnNextpage();

	std::vector<DUIWindow *>	  m_vecWndPtr;			// 保存所有的窗口的地址

private:

	DMSmartPtrT<CMainWnd>         m_pMainWnd;			// 主窗口
	DUIWindow					  *m_pWndTitle;			// 保存标题栏的地址
	CSceneChoose				  *m_pSceneChoose;		// 场景选择对象
	CSceneShoot					  *m_pSceneShoot;		// 场景拍摄对象	

	bool m_strawcolor;									// 吸管是否开启
};


