#pragma once

class DUIDragFrame;
class CShootSystem;
class CPersonPreview;
class CForegroundPreview;
class CBackgroundPreview;
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
	void ChangeSceneShootPreviewBg(std::wstring &filepath);
	void ChangeSceneShootFg(std::string &imgBuf);
	void ChangeSceneShootPreviewFg(std::wstring &filepath);

	void Init(void);
	int ShowSDValue(DUISliderCtrl *, DUIStatic *);
	void ShowStaticColor(COLORREF RGBcolor);
	void HandleRotate(void);
	void HandleEnvirColor(void);
	void HandleForeground(void);
	void HandleImport(void);
	void HandleSDChanged(DMEventArgs *pEvt);
	void HandleReturnBtn();

	// 自定义消息处理
	void ProduceSDMessage(int message);
	void HandleSDMessage(int, int, int, int, int);

private:
	DUIWindow				*m_pWin;
	CShootSystem			*m_pShootSystem;
	CPersonPreview			*m_pPerson;
	CForegroundPreview		*m_pForeground;				// 前景框
	CBackgroundPreview		*m_pBackground;				// 背景框

	DUIStatic				*m_pLightshadeStatic;
	DUISliderCtrl			*m_pLightshadeSlider;

	DUIStatic				*m_pContrastStatic;
	DUISliderCtrl			*m_pContrastSlider;

	DUIStatic				*m_pColourtempStatic;
	DUISliderCtrl			*m_pColourtempSlider;

	DUIStatic				*m_pColourdiffStatic;
	DUISliderCtrl			*m_pColourdiffSlider;
	
	DUIButton				*m_pRotateButton;			// 旋转按钮
	DUIButton				*m_pForegroundButton;		// 前景按钮
	DUIButton				*m_pStrawButton;			// 吸管按钮
	DUIStatic				*m_pColorStatic;			// 环境色

	DUIDragFrame			*m_pDragFrame;				// 拖曳框

	DWORD		m_lastSDTime;							// 记录最后滑动时间

	int m_lightshadenum;
	int m_contrastnum;
	int m_colourtempnum;
	int m_colourdiffnum;
	int m_foregroundstat;

	COLORREF m_EnvirColor;								// 环境色
};


