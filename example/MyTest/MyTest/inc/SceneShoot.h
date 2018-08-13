#pragma once

class CShootSystem;
class PersonPreview;
class CSceneShoot
{
	// 前景状态
	enum FORESTAT
	{
		FOREON,		// 开
		FOREOFF		// 关
	};

public:
	CSceneShoot(CShootSystem *);

	void Init(void);
	int ShowSDValue(DUISliderCtrl *, DUIStatic *);
	void ShowStaticColor(COLORREF RGBcolor);
	void HandleRotate(void);
	void HandleForeground(void);
	void HandleImport(void);
	void HandleSDChanged(DMEventArgs *pEvt);

private:
	DUIWindow * p_Win;
	PersonPreview *p_Person;

	DUIStatic * p_LightshadeStatic;
	DUISliderCtrl *p_LightshadeSlider;

	DUIStatic *p_ContrastStatic;
	DUISliderCtrl *p_ContrastSlider;

	DUIStatic *p_ColourtempStatic;
	DUISliderCtrl *p_ColourtempSlider;

	DUIStatic *p_ColourdiffStatic;
	DUISliderCtrl *p_ColourdiffSlider;

	DUIStatic *p_Staticfg;

	DUIButton *p_RotateButton;			// 旋转按钮
	DUIButton *p_ForegroundButton;		// 前景按钮
	DUIButton *p_StrawButton;			// 吸管按钮
	DUIStatic *p_ColorStatic;			// 环境色

private:
	int m_lightshadenum;
	int m_contrastnum;
	int m_colourtempnum;
	int m_colourdiffnum;
	int m_foregroundstat;

	CShootSystem *pShootSystem;
};


