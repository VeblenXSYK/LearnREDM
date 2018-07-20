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
#include "DMResFolderImpl.h"

class ImagePreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(ImagePreview, L"ImagePreview", DMREG_Window);
public:
	ImagePreview() {};
public:
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
	DM_END_MSG_MAP()
public:
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	void DM_OnPaint(IDMCanvas* pCanvas);

public:
	DMSmartPtrT<IDMSkin>                         m_pSkin;
	CStringW                                     m_strThemeName;
};