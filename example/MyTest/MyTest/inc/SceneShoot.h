#pragma once

class CShootSystem;
class CSceneShoot
{
	// Ç°¾°×´Ì¬
	enum FORESTAT
	{
		FOREON,		// ¿ª
		FOREOFF		// ¹Ø
	};

public:
	CSceneShoot(CShootSystem *);

	void Init(void);
	int ShowSDValue(DUISliderCtrl *, DUIStatic *);
	void HandleForeground(void);
	void HandleImport(void);
	void HandleSDChanged(DMEventArgs *pEvt);

private:
	DUIStatic * p_LightshadeStatic;
	DUISliderCtrl *p_LightshadeSlider;

	DUIStatic *p_ContrastStatic;
	DUISliderCtrl *p_ContrastSlider;

	DUIStatic *p_ColourtempStatic;
	DUISliderCtrl *p_ColourtempSlider;

	DUIStatic *p_ColourdiffStatic;
	DUISliderCtrl *p_ColourdiffSlider;

	DUIButton *p_ForegroundButton;

private:
	int m_lightshadenum;
	int m_contrastnum;
	int m_colourtempnum;
	int m_colourdiffnum;
	int m_foregroundstat;

	CShootSystem *pShootSystem;
};


