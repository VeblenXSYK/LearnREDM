#pragma once

class CShootSystem;
class CSceneShoot
{
	// ǰ��״̬
	enum FORESTAT
	{
		FOREON,		// ��
		FOREOFF		// ��
	};

public:
	CSceneShoot(CShootSystem *);

	void Init(void);
	int ShowSDValue(DUISliderCtrl *, DUIStatic *);
	void ShowStaticColor(COLORREF RGBcolor);
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

	DUIButton *p_ForegroundButton;		// ǰ����ť
	DUIButton *p_StrawButton;			// ���ܰ�ť
	DUIStatic *p_ColorStatic;			// ����ɫ

private:
	int m_lightshadenum;
	int m_contrastnum;
	int m_colourtempnum;
	int m_colourdiffnum;
	int m_foregroundstat;

	CShootSystem *pShootSystem;
};


