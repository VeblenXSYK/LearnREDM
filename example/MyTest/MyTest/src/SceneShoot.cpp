#include "StdAfx.h"
#include "ShootSystem.h"
#include "SceneShoot.h"
#include <commdlg.h>


CSceneShoot::CSceneShoot(CShootSystem *ShootSystem)
{
	pShootSystem = ShootSystem;
	p_LightshadeSlider = NULL;
}

void CSceneShoot::Init(void)
{
	p_LightshadeStatic = pShootSystem->FindChildByNameT<DUIStatic>(L"lightshadestatic");
	p_LightshadeSlider = pShootSystem->FindChildByNameT<DUISliderCtrl>(L"lightshadeslider");

	p_ContrastStatic = pShootSystem->FindChildByNameT<DUIStatic>(L"contraststatic");
	p_ContrastSlider = pShootSystem->FindChildByNameT<DUISliderCtrl>(L"contrastslider");

	p_ColourtempStatic = pShootSystem->FindChildByNameT<DUIStatic>(L"colourtempstatic");
	p_ColourtempSlider = pShootSystem->FindChildByNameT<DUISliderCtrl>(L"colourtempslider");

	p_ColourdiffStatic = pShootSystem->FindChildByNameT<DUIStatic>(L"colourdiffstatic");
	p_ColourdiffSlider = pShootSystem->FindChildByNameT<DUISliderCtrl>(L"colourdiffslider");

	p_ForegroundButton = pShootSystem->FindChildByNameT<DUIButton>(L"sceneshoot_forebtn");

	// 设置明暗
	SetLightshade();
	// 设置对比度
	SetContrast();
	// 设置色温
	SetColourtemp();
	// 设置色差
	SetColourdiff();

	m_foregroundstat = FOREON;
}

void CSceneShoot::SetLightshade(void)
{
	int value = p_LightshadeSlider->m_iValue;

	CStringW strValue;
	if (value < 0)
		strValue.Format(L"%d", value);
	else
		strValue.Format(L"+%d", value);

	p_LightshadeStatic->DV_SetWindowText(strValue);

	m_lightshadenum = value;
}

void CSceneShoot::SetContrast(void)
{
	int value = p_ContrastSlider->m_iValue;

	CStringW strValue;
	if (value < 0)
		strValue.Format(L"%d", value);
	else 
		strValue.Format(L"+%d", value);

	p_ContrastStatic->DV_SetWindowText(strValue);

	m_contrastnum = value;
}

void CSceneShoot::SetColourtemp(void)
{
	int value = p_ColourtempSlider->m_iValue;

	CStringW strValue;
	if (value < 0)
		strValue.Format(L"%d", value);
	else
		strValue.Format(L"+%d", value);

	p_ColourtempStatic->DV_SetWindowText(strValue);

	m_colourtempnum = value;
}

void CSceneShoot::SetColourdiff(void)
{
	int value = p_ColourdiffSlider->m_iValue;

	CStringW strValue;
	if (value < 0)
		strValue.Format(L"%d", value);
	else
		strValue.Format(L"+%d", value);

	p_ColourdiffStatic->DV_SetWindowText(strValue);

	m_colourdiffnum = value;
}

void CSceneShoot::HandleForeground(void)
{
	if (m_foregroundstat == FOREON)
	{
		p_ForegroundButton->SetAttribute(L"skin", L"sceneshootforeoff");
		m_foregroundstat = FOREOFF;
	}
	else
	{
		p_ForegroundButton->SetAttribute(L"skin", L"sceneshootforeon");
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
		LPWSTR lpszPath = L".\\WorldTripShootRes\\outimage\\";
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
		ofn.hwndOwner = pShootSystem->m_hWnd;
		if (::GetOpenFileNameW(&ofn))
		{// todo.hgy413 note:GetOpenFileNameW点击后WM_LBUTTTONUP消息会发送给dui，所以不要随意只处理WM_LBUTTONUP
			;
		}
	} while (false);
}

void CSceneShoot::HandleHScroll(void)
{
	if (p_LightshadeSlider != NULL)
		SetLightshade();
}

