﻿#include "StdAfx.h"
#include "ShootSystem.h"
#include "PersonPreview.h"
#include "DUIDragFrame.h"
#include "SceneShoot.h"

#include <commdlg.h>


CSceneShoot::CSceneShoot(CShootSystem *pShootSystem)
{
	m_pShootSystem = pShootSystem;
	m_pPerson = NULL;
}

void CSceneShoot::Init(void)
{	
	m_pWin = m_pShootSystem->FindChildByNameT<DUIWindow>(L"sceneshoot_win");

	m_pLightshadeStatic = m_pShootSystem->FindChildByNameT<DUIStatic>(L"lightshadestatic");
	m_pLightshadeSlider = m_pShootSystem->FindChildByNameT<DUISliderCtrl>(L"sceneshoot_lightshadeslider");

	m_pContrastStatic = m_pShootSystem->FindChildByNameT<DUIStatic>(L"contraststatic");
	m_pContrastSlider = m_pShootSystem->FindChildByNameT<DUISliderCtrl>(L"sceneshoot_contrastslider");

	m_pColourtempStatic = m_pShootSystem->FindChildByNameT<DUIStatic>(L"colourtempstatic");
	m_pColourtempSlider = m_pShootSystem->FindChildByNameT<DUISliderCtrl>(L"sceneshoot_colourtempslider");

	m_pColourdiffStatic = m_pShootSystem->FindChildByNameT<DUIStatic>(L"colourdiffstatic");
	m_pColourdiffSlider = m_pShootSystem->FindChildByNameT<DUISliderCtrl>(L"sceneshoot_colourdiffslider");

	m_pRotateButton = m_pShootSystem->FindChildByNameT<DUIButton>(L"sceneshoot_rotatebtn");
	m_pForegroundButton = m_pShootSystem->FindChildByNameT<DUIButton>(L"sceneshoot_forebtn");
	m_pStrawButton = m_pShootSystem->FindChildByNameT<DUIButton>(L"sceneshoot_strawbtn");
	m_pColorStatic = m_pShootSystem->FindChildByNameT<DUIStatic>(L"sceneshoot_colorstatic");

	// 拖动框
	m_pDragFrame = m_pShootSystem->FindChildByNameT<DUIDragFrame>(L"ds_dragframe");
	m_pDragFrame->p_Parent = m_pWin;

	// 前景控件
	m_pStaticfg = m_pShootSystem->FindChildByNameT<DUIStatic>(L"sceneshoot_staticfg");

	// 显示明暗
	m_lightshadenum = ShowSDValue(m_pLightshadeSlider, m_pLightshadeStatic);
	// 显示对比度
	m_contrastnum = ShowSDValue(m_pContrastSlider, m_pContrastStatic);
	// 显示色温
	m_colourtempnum = ShowSDValue(m_pColourtempSlider, m_pColourtempStatic);
	// 显示色差
	m_colourdiffnum = ShowSDValue(m_pColourdiffSlider, m_pColourdiffStatic);

	// 默认前景开
	m_foregroundstat = FOREON;
}

int CSceneShoot::ShowSDValue(DUISliderCtrl *pSlider, DUIStatic *pStatic)
{
	// 获取SliderCtrl当前值
	int value = pSlider->m_iValue;

	CStringW strValue;
	if (value <= 0)
		strValue.Format(L"%d", value);
	else
		strValue.Format(L"+%d", value);

	// 在Static控件中显示当前值
	pStatic->DV_SetWindowText(strValue);

	return value;
}

void CSceneShoot::ShowStaticColor(COLORREF RGBcolor)
{
	int red = GetRValue(RGBcolor);
	int green = GetGValue(RGBcolor);
	int blue = GetBValue(RGBcolor);

	CStringW strClrbg;
	strClrbg.Format(L"pbgra(%x,%x,%x,ff)", blue, green, red);
	m_pColorStatic->SetAttribute(L"clrbg", strClrbg);
}

void CSceneShoot::HandleRotate(void)
{
	if(m_pPerson != NULL)
		m_pPerson->ModifyAngle();
}

void CSceneShoot::HandleForeground(void)
{
	if (m_foregroundstat == FOREON)
	{
		m_pForegroundButton->SetAttribute(L"skin", L"sceneshootforeoff");
		m_pStaticfg->SetAttribute(L"alpha", L"0");
		m_foregroundstat = FOREOFF;
	}
	else
	{
		m_pForegroundButton->SetAttribute(L"skin", L"sceneshootforeon");
		m_pStaticfg->SetAttribute(L"alpha", L"50");
		m_foregroundstat = FOREON;
	}
}

//LRESULT CALLBACK OFNHookProc(HWND hWnd, UINT uMessage, WPARAM wParam, LPARAM lParam)
//{
//	HWND hhwnd = ::GetParent(hWnd);
//	if (uMessage == WM_INITDIALOG)
//	{
//		::SetDlgItemText(hhwnd, 1, _T("hello >>"));
//		return TRUE;
//	}
//}

void CSceneShoot::HandleImport(void)
{
	do
	{
		LPWSTR lpszPath = L".\\WorldTripShootRes\\outimage\\Person\\";
		wchar_t szPath[MAX_PATH] = { 0 };
		GetRootFullPath(lpszPath, szPath, MAX_PATH);

		wchar_t szFileTitle[_MAX_FNAME] = { 0 };
		wchar_t szFileName[_MAX_PATH] = { 0 };
		OPENFILENAMEW ofn;
		memset(&ofn, 0, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.lpstrFile = szFileName;
		ofn.nMaxFile = _MAX_PATH;
		ofn.lpstrDefExt = L"gif";
		ofn.lpstrInitialDir = szPath;
		ofn.lpstrFileTitle = (LPTSTR)szFileTitle;
		ofn.nMaxFileTitle = _MAX_FNAME;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER;
		//ofn.lpfnHook = (LPOFNHOOKPROC)OFNHookProc;
		//ofn.Flags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING | OFN_NOCHANGEDIR;
		ofn.lpstrFilter = L"图像文件(*.bmp;*.jpg;*.png)\0*.bmp;*.jpg;*.png\0\0";
		ofn.hwndOwner = m_pShootSystem->m_hWnd;
		if (::GetOpenFileNameW(&ofn))
		{// todo.hgy413 note:GetOpenFileNameW点击后WM_LBUTTTONUP消息会发送给dui，所以不要随意只处理WM_LBUTTONUP
			if (m_pPerson == NULL)
			{
				g_pDMApp->CreateRegObj((void**)&m_pPerson, L"PersonPreview", DMREG_Window);
				if (m_pPerson)
				{
					m_pPerson->SetAttribute(L"pos", L"|0,|0,@288,@493");
					m_pPerson->m_pSkin = NULL;
					m_pPerson->p_Parent = m_pWin;
					m_pWin->DM_InsertChild(m_pPerson);
					m_pDragFrame->DM_SetWndToTop();
				}
			}
			m_pPerson->LoadImage(szFileName);
			m_pPerson->m_picPath = szFileName;
			m_pWin->DV_UpdateChildLayout();

			CRect rcFrame;
			m_pDragFrame->InitDragFrame(m_pPerson, rcFrame);
			m_pDragFrame->DM_FloatLayout(rcFrame);
			m_pDragFrame->DM_SetVisible(true, true);
			m_pWin->DM_Invalidate();
		}
	} while (false);
}

void CSceneShoot::HandleSDChanged(DMEventArgs *pEvt)
{
	if(0 == _wcsicmp(pEvt->m_szNameFrom, L"sceneshoot_lightshadeslider"))
		m_lightshadenum = ShowSDValue(m_pLightshadeSlider, m_pLightshadeStatic);
	else if(0 == _wcsicmp(pEvt->m_szNameFrom, L"sceneshoot_contrastslider"))
		m_contrastnum = ShowSDValue(m_pContrastSlider, m_pContrastStatic);
	else if(0 == _wcsicmp(pEvt->m_szNameFrom, L"sceneshoot_colourtempslider"))
		m_colourtempnum = ShowSDValue(m_pColourtempSlider, m_pColourtempStatic);
	else
		m_colourdiffnum = ShowSDValue(m_pColourdiffSlider, m_pColourdiffStatic);
}

