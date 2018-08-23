#pragma once

class DUIDragFrame;
class CShootSystem;
class CPersonPreview;
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
	void ChangeSceneShootBg(std::string &imgBuf);
	void ChangeSceneShootFg(std::string &imgBuf);

	void Init(void);
	int ShowSDValue(DUISliderCtrl *, DUIStatic *);
	void ShowStaticColor(COLORREF RGBcolor);
	void HandleRotate(void);
	void HandleEnvirColor(void);
	void HandleForeground(void);
	void HandleImport(void);
	void HandleSDChanged(DMEventArgs *pEvt);

	// 自定义消息处理
	void ProduceSDMessage(int message);
	void HandleSDMessage(int, int, int, int, int);

private:
	DUIWindow				*m_pWin;
	CShootSystem			*m_pShootSystem;
	CPersonPreview			*m_pPerson;

	DUIStatic				*m_pLightshadeStatic;
	DUISliderCtrl			*m_pLightshadeSlider;

	DUIStatic				*m_pContrastStatic;
	DUISliderCtrl			*m_pContrastSlider;

	DUIStatic				*m_pColourtempStatic;
	DUISliderCtrl			*m_pColourtempSlider;

	DUIStatic				*m_pColourdiffStatic;
	DUISliderCtrl			*m_pColourdiffSlider;

	DUIStatic				*m_pStaticfg;				// 前景框
	DUIButton				*m_pRotateButton;			// 旋转按钮
	DUIButton				*m_pForegroundButton;		// 前景按钮
	DUIButton				*m_pStrawButton;			// 吸管按钮
	DUIStatic				*m_pColorStatic;			// 环境色

	DUIDragFrame			*m_pDragFrame;				// 拖曳框

	int m_lightshadenum;
	int m_contrastnum;
	int m_colourtempnum;
	int m_colourdiffnum;
	int m_foregroundstat;

	COLORREF m_EnvirColor;								// 环境色
};


