#pragma once

class CMainWnd;
class CSceneChoose : public DMHWnd
{
public:
	CSceneChoose(CMainWnd *pMainWnd);


	DECLARE_MESSAGE_MAP()// 仿MFC消息映射宏
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	//void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnReturn();

private:
	DMSmartPtrT<CMainWnd>         m_pMainWnd;
};