#include "StdAfx.h"
#include "ImagePreview.h"


void ImagePreview::DM_OnPaint(IDMCanvas* pCanvas)
{
	if (m_pSkin)
	{
		m_pSkin->Draw(pCanvas, m_rcWindow, 0);
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
