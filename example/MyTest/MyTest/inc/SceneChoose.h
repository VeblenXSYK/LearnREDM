#pragma once

class CMainWnd;
class CSceneChoose : public DMHWnd
{
public:
	CSceneChoose(CMainWnd *pMainWnd);


	DECLARE_MESSAGE_MAP()// ��MFC��Ϣӳ���
	DECLARE_EVENT_MAP()

	//---------------------------------------------------
	// Function Des: ��Ϣ�ַ�ϵ�к���
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	//void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: �¼��ַ�ϵ�к���
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnReturn();

private:
	DMSmartPtrT<CMainWnd>         m_pMainWnd;
};