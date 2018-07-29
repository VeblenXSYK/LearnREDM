#pragma once

class CShootSystem;
class CSceneShoot
{
	enum FORESTAT
	{
		FOREON,
		FOREOFF
	};

public:
	CSceneShoot(CShootSystem *);

	void Init(void);
	void SetLightshade(void);
	void SetContrast(void);
	void SetColourtemp(void);
	void SetColourdiff(void);
	void HandleForeground(void);
	void HandleImport(void);
	void HandleHScroll(void);

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


