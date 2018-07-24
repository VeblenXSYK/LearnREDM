#include "StdAfx.h"
#include "MainWnd.h"
#include "ImagePreview.h"

int APIENTRY _tWinMain(HINSTANCE hInstance,
					   HINSTANCE hPrevInstance,
					   LPTSTR    lpCmdLine,
					   int       nCmdShow)
{   
	OleInitialize(NULL);
	DMApp theApp(hInstance);												// ����ȫ��Ψһ����theApp��֮��ʹ��g_pDMApp������ 

	theApp.LoadResPack((WPARAM)(L"WorldTripShootRes"),NULL,NULL);			// ·���������������Ŀ¼
	theApp.InitGlobal();													// ��ʼ��ָ����ȫ��skin��style��Ĭ������
	theApp.Register(DMRegHelperT<ImagePreview>(), true);					// ע���Զ���ؼ�

	DMSmartPtrT<CMainWnd> pMainWnd;
	pMainWnd.Attach(new CMainWnd());
	pMainWnd->DM_CreateWindow(L"main",0,0,0,0,NULL,false);					// ���������ڣ�main��ӦMyRes��Դ�а���main�ı�ʶ��

	pMainWnd->SendMessage(WM_INITDIALOG);
	pMainWnd->CenterWindow();
	pMainWnd->ShowWindow(SW_SHOW);
	theApp.Run(pMainWnd->GetSafeHwnd());								    // ���е�ǰ�̵߳���Ϣѭ������������Ϣ���й�����

	return (int) 0;
}
