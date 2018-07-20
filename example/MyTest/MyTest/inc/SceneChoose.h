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

	static const int PicNumOfPage = 15;					// WrapLayout中一页的图片数目
	std::vector<ImagePreview *> m_vecChildPtr_Wrap;		// 保存WrapLayout中所有ImagePreview子控件的地址
	int m_curPageNum_Wrap;								// WrapLayout中的当前显示页
	int m_curShowPicType;								// 当前显示的图片类型

	wchar_t m_wcPicDirPath_European[MAX_PATH];			// 欧式简约图片目录路径
	wchar_t m_wcPicDirPath_Pastoral[MAX_PATH];			// 唯美田园图片目录路径
	std::vector<std::wstring> m_vecPicPath_European;	// 保存欧式简约所有图片的路径
	std::vector<std::wstring> m_vecPicPath_Pastoral;	// 保存唯美田园所有图片的路径
};
