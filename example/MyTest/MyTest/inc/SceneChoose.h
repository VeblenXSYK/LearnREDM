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
	DMCode HandlePrepage();
	DMCode HandleNextpage();

	// 在WrapLayout中显示图片
	void ShowImageOfWrap(std::set<std::wstring> &, int default = 0);
	// 清除WrapLayout中的图片
	void ClearImageOfWrap(void);
	// 获取目标路径下指定格式的全部图片路径
	void GetFilePathOfFmt(CStringW dirpath, std::set<std::wstring> &, std::wstring);

	/*
		自定义消息处理
	*/
	// 解析psd消息
	void DefParsePsdMessage(std::wstring);

	DUITreeCtrl					  *m_pTreeCtrl;	
	DUIWrapLayout				  *m_pWrapLayout;
	DUIListBoxEx				  *m_pListBoxEx;
	CImagePreview				  *m_pSelImage;			// 当前选中的图片控件
	CShootSystem				  *m_pShootSystem;		// 拍摄系统
	HDMTREEITEM					  m_hSelItem_tree;		// 保存选取的tree项

private:

	enum SHOOTMODE
	{
		ONE_CHOOSE,
		ALL_CHOOSE,
		UNDEF_CHOOSE
	};

	static const int PICHEIGHT_ONE = 200;				// 每张图片的高度
	static const int PICHEIGHT_AREA = 600;				// 图片区域的高度
	std::vector<CImagePreview *> m_vecChildPtr_Wrap;		// 保存WrapLayout中所有CImagePreview子控件的地址

	// 图片信息 <图片目录路径名，该路径下所有文件路径名>
	std::map<CStringW, std::shared_ptr<std::set<std::wstring>>> m_mapPicInfo;
	// “全部拍摄”下指向当前场景的迭代器
	std::set<std::wstring>::iterator m_ItorAllShoot;
	// “全部拍摄”下指向所有psd文件目录名的地址
	std::shared_ptr<std::set<std::wstring>> m_pPathAllShoot;
	// 拍摄模式（“选择拍摄”与“全部拍摄”）
	int m_shootmode;
};


