//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: CBackgroundPreview.h 
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

class CBackgroundPreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(CBackgroundPreview, L"BackgroundPreview", DMREG_Window);
public:
	CBackgroundPreview() {};
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