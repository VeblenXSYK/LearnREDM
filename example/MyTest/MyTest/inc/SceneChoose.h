#pragma once

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>

class CShootSystem;
class CImagePreview;
class CSceneChoose
{
public:
	CSceneChoose::CSceneChoose(CShootSystem *);

	void Init(void);
	DMCode HandleTreeSelChanged(DMEventArgs *pEvt);
	DMCode HandleAddPreChoose();
	DMCode HandleDelPreChoose();
	DMCode HandleOneShootChoose();
	DMCode HandleAllShootChoose();
	DMCode HandlePrevpage(int);
	DMCode HandleNextpage(int);

	// 在WrapLayout中显示图片
	void ShowImageOfWrap(std::set<std::wstring> &, int default = 0);
	// 清除WrapLayout中的图片
	void ClearImageOfWrap(void);
	// 获取目标路径下指定格式的全部图片路径
	void GetFilePathOfFmt(CStringW dirpath, std::set<std::wstring> &, std::wstring);
	// 产生解析Psd的消息
	void ProduceParsePsdMessage(std::wstring &path, int message);
	// 打开Psd文件
	void OpenPsdFile(std::wstring &);
	// 获取Psd背景图片数据
	void GetBackgroundLayerImageData(void);
	// 获取Psd前景图片数据
	void GetForegroundLayerImageData(void);

	/*
		自定义消息处理
	*/
	void HandleParsePsdMessage(std::wstring, int messagetype);

	DUITreeCtrl					  *m_pTreeCtrl;	
	DUIWrapLayout				  *m_pWrapLayout;
	DUIListBoxEx				  *m_pListBoxEx;
	CImagePreview				  *m_pSelImage;			// 当前选中的图片控件
	CShootSystem				  *m_pShootSystem;		// 拍摄系统
	HDMTREEITEM					  m_hSelItem_tree;		// 保存选取的tree项
	std::string					  m_curFgImage;			// 当前前景图片数据
	std::string					  m_curBgImage;			// 当前背景图片数据
	std::wstring				  m_curImagepath;		// 当前场景路径
	std::wstring				  m_curFgPreviewPath;	// 当前前景预览图片路径
	std::wstring				  m_curBgPreviewPath;	// 当前背景预览图片路径

private:

	enum SHOOTMODE
	{
		ONE_CHOOSE,
		ALL_CHOOSE,
		UNDEF_CHOOSE
	};

	static const int PICHEIGHT_ONE = 200;				// 每张图片的高度
	static const int PICHEIGHT_AREA = 600;				// 图片区域的高度
	std::vector<CImagePreview *> m_vecChildPtr_Wrap;	// 保存WrapLayout中所有CImagePreview子控件的地址

	// 图片信息 <图片目录路径名，该路径下所有文件路径名>
	std::map<CStringW, std::shared_ptr<std::set<std::wstring>>> m_mapPicInfo;
	// “全部拍摄”下指向当前场景的迭代器
	std::set<std::wstring>::iterator m_ItorAllShoot;
	// “全部拍摄”下指向所有psd文件目录名的地址
	std::shared_ptr<std::set<std::wstring>> m_pPathAllShoot;
	// 拍摄模式（“选择拍摄”与“全部拍摄”）
	int m_shootmode;
};


