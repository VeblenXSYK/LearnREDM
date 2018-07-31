#include "StdAfx.h"
#include "ShootSystem.h"
#include "ImagePreview.h"

void ImagePreview::DM_OnPaint(IDMCanvas* pCanvas)
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

		// 判断是否为当前选中控件
		if (this == m_pShootSystem->p_SelImage)
		{
			CRect rcDraw = rcItem;
			rcDraw.DeflateRect(0, 1, 0, 1);

			pCanvas->FillSolidRect(rcDraw, PBGRA(0, 0, 0, 0X80));
		}	
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

void ImagePreview::OnLButtonDown(UINT nFlags, CPoint pt)
{
	__super::OnLButtonDown(nFlags, pt);

	// 更新之前选中的控件
	if (m_pShootSystem->p_SelImage != NULL)
		m_pShootSystem->p_SelImage->DM_Invalidate();
	// 设置当前选中的控件
	m_pShootSystem->p_SelImage = this;
}

