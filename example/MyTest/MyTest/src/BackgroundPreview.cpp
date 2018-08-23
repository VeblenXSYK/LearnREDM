#include "StdAfx.h"
#include "BackgroundPreview.h"

void CBackgroundPreview::DM_OnPaint(IDMCanvas* pCanvas)
{
	if (m_pSkin)
	{
		// 获取图片的尺寸
		CSize sz;
		m_pSkin->GetStateSize(sz);

		// 获取控件的尺寸
		CRect rcItem;
		this->DV_GetClientRect(&rcItem);

		// 把图片按照控件等比例缩放
		MeetRect(rcItem, sz);

		m_pSkin->Draw(pCanvas, rcItem, 0);
	}
	else
	{
		DMColor ClrBg;
		m_pDUIXmlInfo->m_pStyle->GetBgColor(0, ClrBg);
		if (!ClrBg.IsTextInvalid())
		{
			pCanvas->FillSolidRect(&m_rcWindow, ClrBg);
		}
	}
}

