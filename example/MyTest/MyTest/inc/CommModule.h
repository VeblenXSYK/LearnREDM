#pragma once
#include "string"

class CCommModule
{
public:
	void LoadResource(void);
	static wchar_t *GetPicRootDir(void);
	static CCommModule *GetInstance(void);
private:
	CCommModule() {}
	~CCommModule() {};
	static CCommModule *m_Instance;
	static wchar_t m_PicRootDir[MAX_PATH];	// 图片根目录
};


