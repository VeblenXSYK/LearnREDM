//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: PersonPreview.h 
// File Des: 预览窗口
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-14	1.0		
//-------------------------------------------------------
#pragma once

#include <string>
#include "DMResFolderImpl.h"
#include "Gdiplus.h"

class CShootSystem;
class PersonPreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(PersonPreview, L"PersonPreview", DMREG_Window);
public:
	PersonPreview();
	~PersonPreview();
	void Rotate(Gdiplus::Graphics &graphics, float angle);
	void ModifyAngle(void);

	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	DM_END_MSG_MAP()

	enum MouseMode 
	{ 
		NoneMode = -1, 
		MoveMode, 
		ZoomMode
	};

public:
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	void DM_OnPaint(IDMCanvas* pCanvas);
	void OnLButtonDown(UINT nFlags, CPoint pt);
	void OnLButtonUp(UINT nFlags, CPoint pt);
	void OnMouseMove(UINT nFlags, CPoint pt);

public:
	DMSmartPtrT<IDMSkin>          m_pSkin;
	std::wstring				  m_picPath;			// 保存该控件对应图片的路径
	DUIWindow					  *p_Parent;			// 父窗口

	// 拖动操作
	bool						  m_bDown;				// 鼠标按下
	int							  m_iMode;				// 模式
	CPoint						  m_StartDragPt;
	CPoint						  m_TrackDragPt;
	CRect						  m_StartDragRc;

private:
	void OnDragLeft(CPoint pt);			// 拖动左边
	void OnDragLeftTop(CPoint pt);		// 拖动左上角
	void OnDragTop(CPoint pt);			// 拖动上边
	void OnDragRightTop(CPoint pt);		// 拖动右上角
	void OnDragRight(CPoint pt);		// 拖动右边
	void OnDragRightBottom(CPoint pt);	// 拖动右下角
	void OnDragBottom(CPoint pt);		// 拖动下边
	void OnDragLeftBottom(CPoint pt);	// 拖动左下角
	ULONG_PTR gdiplusToken;
	float m_AngleOfRotation;			// 旋转角度
};