#pragma once

#include "string"
#include "list"
#include "mutex"
#include "condition_variable"

/*
	GUI消息类
*/
class CGuiMessage
{
private:
	typedef std::function<void(void)> Callback;

public:

	enum GuiMessageType
	{
		SECENECHOOSE_PARSEPSD_ONE,
		SECENECHOOSE_PARSEPSD_ALL,
		SECENEDETAIL_PARSEPSD_PREV,
		SECENEDETAIL_PARSEPSD_NEXT,
		SECENEDETAIL_PARSEPSD_SHOOT,
		SECENESHOOT_PARSEPSD_PREV,
		SECENESHOOT_PARSEPSD_NEXT,
		SECENESHOOT_SETLIGHT_SD,
		SECENESHOOT_SETCONTRAST_SD,
		SECENESHOOT_SETCOLORTEMP_SD,
		SECENESHOOT_SETCOLORDIFF_SD
	};

	CGuiMessage() {}
	~CGuiMessage() {}

	void SetMessageCallback(const Callback& cb)
	{
		m_MessageCallback = cb;
	}

	Callback m_MessageCallback;			// 消息回调函数
};

/*
	任务队列类
*/
template<typename T>
class CTaskQueue {
public:
	CTaskQueue(unsigned int size = 0) :m_list_size(size > 0 ? size : INT_MAX) {}

	~CTaskQueue() {}

	void Push(const T& element) 
	{
		std::unique_lock<std::mutex> lock(m_block_mutex);
		m_full_notify.wait(m_block_mutex, [this]() {return this->m_block_queue.size() < this->m_list_size; });
		m_block_queue.push_front(element);
		m_empty_notify.notify_all();
	}

	T Pop() 
	{
		std::unique_lock<std::mutex> lock(m_block_mutex);
		m_empty_notify.wait(m_block_mutex, [this]() {return !this->m_block_queue.empty(); });
		T ret = std::move(m_block_queue.back());
		m_block_queue.pop_back();
		m_full_notify.notify_all();
		return std::move(ret);
	}

	void Clear(bool notify = true)
	{
		std::unique_lock<std::mutex> lock(m_block_mutex);
		while (!m_block_queue.empty()) 
		{
			m_block_queue.pop_front();
			m_full_notify.notify_one();
		}
	}

	int Size() 
	{
		std::unique_lock<std::mutex> lock(m_block_mutex);
		return m_block_queue.size();
	}

private:
	unsigned int                    m_list_size;
	std::list<T>                    m_block_queue;
	std::mutex                      m_block_mutex;
	std::condition_variable_any     m_empty_notify;
	std::condition_variable_any     m_full_notify;
};
// 消息任务队列
extern CTaskQueue<CGuiMessage>	g_MessageTaskQueue;

/*
	工作线程类
*/
class CWorkThread
{
public:
	void Start(void);
private:
	static UINT *HandleGuiMessage(void *lpParameter);
};

/*
	通用模块类
*/
class CCommModule
{
public:
	void LoadResource(void);
	static std::string WS2S(const std::wstring ws);
	static std::wstring S2WS(const std::string& s);
	static std::string GetRawString(std::string &str);
	static wchar_t *GetPicRootDir(void);
	static CCommModule *GetInstance(void);
	static void *GetPSHandle(void);
private:
	CCommModule() {}
	~CCommModule() {};
	static CCommModule *m_pInstance;
	static void *m_pPSHandle;				// PSCore 句柄
	static wchar_t m_PicRootDir[MAX_PATH];	// 所有图片根目录
};


