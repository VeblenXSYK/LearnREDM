#include "StdAfx.h"
#include "ShootSystem.h"
#include "SceneShoot.h"
#include <commdlg.h>


CSceneShoot::CSceneShoot(CShootSystem *pShoot)
{
	pShootSystem = pShoot;
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

	// 显示明暗
	m_lightshadenum = ShowSDValue(p_LightshadeSlider, p_LightshadeStatic);
	// 显示对比度
	m_contrastnum = ShowSDValue(p_ContrastSlider, p_ContrastStatic);
	// 显示色温
	m_colourtempnum = ShowSDValue(p_ColourtempSlider, p_ColourtempStatic);
	// 显示色差
	m_colourdiffnum = ShowSDValue(p_ColourdiffSlider, p_ColourdiffStatic);

	// 前景开
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

void CSceneShoot::HandleSDChanged(DMEventArgs *pEvt)
{
	if(0 == _wcsicmp(pEvt->m_szNameFrom, L"lightshadeslider"))
		m_lightshadenum = ShowSDValue(p_LightshadeSlider, p_LightshadeStatic);
	else if(0 == _wcsicmp(pEvt->m_szNameFrom, L"contrastslider"))
		m_contrastnum = ShowSDValue(p_ContrastSlider, p_ContrastStatic);
	else if(0 == _wcsicmp(pEvt->m_szNameFrom, L"colourtempslider"))
		m_colourtempnum = ShowSDValue(p_ColourtempSlider, p_ColourtempStatic);
	else
		m_colourdiffnum = ShowSDValue(p_ColourdiffSlider, p_ColourdiffStatic);
}

