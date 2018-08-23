//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: CForegroundPreview.h 
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

class CForegroundPreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(CForegroundPreview, L"ForegroundPreview", DMREG_Window);
public:
	CForegroundPreview() {};
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
	DM_END_MSG_MAP()

public:
	//---------------------------------------------------
	// Function Des: DUI的消息分发系列函数
	//---------------------------------------------------
	void DM_OnPaint(IDMCanvas* pCanvas);

public:
	DMSmartPtrT<IDMSkin>          m_pSkin;
};