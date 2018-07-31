//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: ImagePreview.h 
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

class CShootSystem;
class ImagePreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(ImagePreview, L"ImagePreview", DMREG_Window);
public:
	ImagePreview() {};
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
	DMSmartPtrT<IDMSkin>          m_pSkin;
	DMSmartPtrT<CShootSystem>	  m_pShootSystem;
	std::wstring				  m_picPath;			// 保存该控件对应图片的路径
};