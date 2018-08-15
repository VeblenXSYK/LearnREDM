//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: CPersonPreview.h 
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
#include <memory>
#include "DMResFolderImpl.h"
#include "Gdiplus.h"

class CShootSystem;
class CPersonPreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(CPersonPreview, L"PersonPreview", DMREG_Window);
public:
	CPersonPreview();
	~CPersonPreview();

	enum MouseAction
	{
		DragLeft,
		DragLeftTop,
		DragTop,
		DragRightTop,
		DragRight,
		DragRightBottom,
		DragBottom,
		DragLeftBottom,
		MoveMode,
		NoneMode
	};

	void Rotate(Gdiplus::Graphics &graphics, float angle);
	void ModifyAngle(void);
	void LoadImage(wchar_t *);
	int JudgeDragType(CPoint pt);
	void HandleDrag(CPoint pt, int type);

	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	DM_END_MSG_MAP()

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
	ULONG_PTR gdiplusToken;
	// 旋转角度
	float m_AngleOfRotation;

	// 图片数据
	std::shared_ptr<Gdiplus::Image> m_pImgData;

	// 默认识别宽度
};