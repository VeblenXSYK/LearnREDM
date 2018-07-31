#include "StdAfx.h"
#include "ShootSystem.h"
#include "ImagePreview.h"

void ImagePreview::DM_OnPaint(IDMCanvas* pCanvas)
{
	if (m_pSkin)
	{
		// ��ȡͼƬ�ĳߴ�
		CSize sz;
		m_pSkin->GetStateSize(sz);

		// ��ȡ�ؼ��ĳߴ�
		CRect rcItem;
		this->DV_GetClientRect(&rcItem);

		// ��ͼƬ���տؼ��ȱ�������
		MeetRect(rcItem, sz);

		m_pSkin->Draw(pCanvas, rcItem, 0);

		// �ж��Ƿ�Ϊ��ǰѡ�пؼ�
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

	// ����֮ǰѡ�еĿؼ�
	if (m_pShootSystem->p_SelImage != NULL)
		m_pShootSystem->p_SelImage->DM_Invalidate();
	// ���õ�ǰѡ�еĿؼ�
	m_pShootSystem->p_SelImage = this;
}

