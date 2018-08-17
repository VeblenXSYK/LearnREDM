#include "StdAfx.h"
#include "CommModule.h"

#include <process.h>


CTaskQueue<CGuiMessage>	g_MessageTaskQueue;
CCommModule *CCommModule::m_pInstance = NULL;
void *CCommModule::m_pPSHandle = NULL;

wchar_t CCommModule::m_PicRootDir[MAX_PATH] = { 0 };

CCommModule *CCommModule::GetInstance(void)
{
	if (m_pInstance == NULL)
	{
		m_pInstance = new CCommModule();
	}

	return m_pInstance;
}

void *CCommModule::GetPSHandle(void)
{
	return m_pPSHandle;
}

void CCommModule::LoadResource(void)
{
	// 填充图片根目录
	memset(m_PicRootDir, 0, MAX_PATH);
	GetRootFullPath(L".\\WorldTripShootRes\\outimage\\", m_PicRootDir, MAX_PATH);

	// 初始化PSCore
	if ((m_pPSHandle = PSInit()) == NULL)
	{
		LOG_USER("PSInit Failed\n");
		return;
	}

	// 登陆
	std::string serverip = "127.0.0.1";
	std::string passwd = "123456";
	if (PSLogin(m_pPSHandle, serverip, passwd) != 0)
	{
		LOG_USER("PSLogin Failed\n");
		return;
	}
}

std::string CCommModule::WS2S(const std::wstring ws)
{
	std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
	setlocale(LC_ALL, "chs");
	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char *_Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	std::string result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return result;
}

std::wstring CCommModule::S2WS(const std::string& s)
{
	setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t *_Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	mbstowcs(_Dest, _Source, _Dsize);
	std::wstring result = _Dest;
	delete[]_Dest;
	setlocale(LC_ALL, "C");
	return result;
}

std::string CCommModule::GetRawString(std::string &str)
{
	std::string::size_type pos = 0;

	while ((pos = str.find_first_of('\\', pos)) != std::string::npos)
	{
		str.insert(pos, "\\");
		pos = pos + 2;
	}

	return str;
}

wchar_t *CCommModule::GetPicRootDir(void)
{
	return m_PicRootDir;
}

void CWorkThread::Start(void)
{
	HANDLE hThread = (HANDLE)_beginthreadex(NULL, NULL, (_beginthreadex_proc_type)CWorkThread::HandleGuiMessage, (LPVOID)(this), 0, 0);
	CloseHandle(hThread); 
}

UINT *CWorkThread::HandleGuiMessage(void *lpParameter)
{
	while (1)
	{
		while (g_MessageTaskQueue.Size() > 0)
		{
			LOG_USER("CWorkThread HandleGUIMessage\n");

			// 从消息队列中取出消息
			CGuiMessage gm = g_MessageTaskQueue.Pop();
			
			// 回调消息函数
			gm.m_MessageCallback();
		}

		Sleep(50);
	}
}
