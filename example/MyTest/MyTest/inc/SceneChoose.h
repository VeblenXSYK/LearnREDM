#pragma once

#include <vector>
#include <string>

#include "DUITreeCtrl.h"

class CMainWnd;
class ImagePreview;
class CSceneChoose : public DMHWnd
{
public:
	CSceneChoose(CMainWnd *pMainWnd);
	void ShowImageOfWrap(int sindex);
	void ClearImageOfWrap(void);
	void GetFilePathOfFmt(std::wstring dirpath, std::vector<std::wstring> &files, std::wstring fmt);

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
	DMCode OnTreeSelChanged(DMEventArgs *pEvt);
	DMCode OnReturn();
	DMCode OnPrepage();
	DMCode OnNextpage();
	DMCode OnClose();

private:
	DUITreeCtrl					  *m_pTreeCtrl;
	DMSmartPtrT<CMainWnd>         m_pMainWnd;
	DUIWindow					  *pWrapLayout;

	enum PICTYPE
	{
		EUROPEAN,
		PASTORAL
	};

	static const int PicNumOfPage = 15;					// WrapLayout��һҳ��ͼƬ��Ŀ
	std::vector<ImagePreview *> m_vecChildPtr_Wrap;		// ����WrapLayout������ImagePreview�ӿؼ��ĵ�ַ
	int m_curPageNum_Wrap;								// WrapLayout�еĵ�ǰ��ʾҳ
	int m_curShowPicType;								// ��ǰ��ʾ��ͼƬ����

	wchar_t m_wcPicDirPath_European[MAX_PATH];			// ŷʽ��ԼͼƬĿ¼·��
	wchar_t m_wcPicDirPath_Pastoral[MAX_PATH];			// Ψ����԰ͼƬĿ¼·��
	std::vector<std::wstring> m_vecPicPath_European;	// ����ŷʽ��Լ����ͼƬ��·��
	std::vector<std::wstring> m_vecPicPath_Pastoral;	// ����Ψ����԰����ͼƬ��·��
};
