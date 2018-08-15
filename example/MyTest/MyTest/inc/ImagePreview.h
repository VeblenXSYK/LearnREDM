//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: CImagePreview.h 
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

class CSceneChoose;
class CImagePreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(CImagePreview, L"ImagePreview", DMREG_Window);
public:
	CImagePreview() {};
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
	DM_END_MSG_MAP()

public:
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	void DM_OnPaint(IDMCanvas* pCanvas);
	void OnLButtonDown(UINT nFlags, CPoint pt);

public:
	std::wstring				  m_picPath;			// 保存该控件对应图片的路径
	DMSmartPtrT<IDMSkin>          m_pSkin;
	CSceneChoose				  *m_pSceneChoose;
};