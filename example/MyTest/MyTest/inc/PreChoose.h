﻿#pragma once  

#include <string>

class CSceneChoose;
class CPreChoose : public DMHDialog
{
public:
	CPreChoose::CPreChoose(CSceneChoose *);

	DECLARE_MESSAGE_MAP()						// 仿MFC消息映射宏，也可以使用BEGIN_MSG_MAPT宏使消息处理在头文件
	DECLARE_EVENT_MAP()							// 事件分发映射宏,也可以使用BEGIN_EVENT_MAPT宏使事件处理在头文件

	//---------------------------------------------------
	// Function Des: 消息分发系列函数
	//---------------------------------------------------
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	void OnSize(UINT nType, CSize size);

	//---------------------------------------------------
	// Function Des: 事件分发系列函数
	//---------------------------------------------------
	DMCode OnClose();
	DMCode OnConfirm();
	DMCode OnCancel();

	CStringW m_PreChooseName;

private:
	CSceneChoose  *m_pSceneChoose;
	DUIEdit *pNameEdit;
};