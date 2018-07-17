#include "StdAfx.h"
#include "MainWnd.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{   
	OleInitialize(NULL);
	DMApp theApp(hInstance);   

	theApp.LoadResPack((WPARAM)(L"WorldTripShootRes"),NULL,NULL);			// 路径总是相对于生成目录
	theApp.InitGlobal();													// 初始化指定的全局skin、style、默认字体

	DMSmartPtrT<CMainWnd> pMainWnd;
	pMainWnd.Attach(new CMainWnd());
	pMainWnd->DM_CreateWindow(L"main",0,0,0,0,NULL,false);					// 创建主窗口（main对应MyRes资源中包中main的标识）

	pMainWnd->SendMessage(WM_INITDIALOG);
	pMainWnd->CenterWindow();
	pMainWnd->ShowWindow(SW_SHOW);
	theApp.Run(pMainWnd->GetSafeHwnd());								    // 运行当前线程的消息循环，并加入消息队列管理中

	return (int) 0;
}