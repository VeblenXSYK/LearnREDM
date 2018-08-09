#include "StdAfx.h"
#include "CommModule.h"


CCommModule *CCommModule::m_Instance = NULL;
wchar_t CCommModule::m_PicRootDir[MAX_PATH] = { 0 };

CCommModule *CCommModule::GetInstance(void)
{
	if (m_Instance == NULL)
	{
		m_Instance = new CCommModule();
	}

	return m_Instance;
}

void CCommModule::LoadResource(void)
{
	// 填充图片根目录
	memset(m_PicRootDir, 0, MAX_PATH);
	GetRootFullPath(L".\\WorldTripShootRes\\outimage\\", m_PicRootDir, MAX_PATH);
}

wchar_t *CCommModule::GetPicRootDir(void)
{
	return m_PicRootDir;
}
