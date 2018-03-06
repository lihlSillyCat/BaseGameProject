#include "stdafx.h"
#include <ctime>
#include "TraceManager.h"

//工作线程
void WorkerThreadMain(CTraceManager* pTraceMgr)
{
	if (nullptr == pTraceMgr)
	{
		return;
	}

	while (pTraceMgr->Running())
	{
		pTraceMgr->Update();
	}

	pTraceMgr->OnExit();
}

//日志管理类
CTraceManager::CTraceManager(wchar* wsName):
	m_bRun(false),
	m_pWorkerThread(nullptr),
	m_wsFileName(wsName)
{
	for (size_t i = 0; i < array_size(m_hEvents); i++)
	{
		m_hEvents[i] = INVALID_HANDLE_VALUE;
	}
}

CTraceManager::~CTraceManager()
{
	Shutdown();
}

//开始
bool CTraceManager::Start()
{
	if (m_bRun)
	{
		return true;
	}

	//创建数据事件和退出事件
	m_hEvents[kDataEvent] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (InvalidHandle(m_hEvents[kDataEvent]))
	{
		return false;
	}
	m_hEvents[kExitEvent] = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	if (InvalidHandle(m_hEvents[kExitEvent]))
	{
		safe_close(m_hEvents[kDataEvent]);
		return false;
	}

	//打开文件
	::CreateDirectory(L"Log", NULL);
	if (m_fsLogFile.is_open())
	{
		m_fsLogFile.close();
	}
	std::wstring name = L"Log\\" + m_wsFileName + L".log";
	m_fsLogFile.open(name, std::wofstream::out | std::wofstream::app | std::wofstream::binary);
	if (!m_fsLogFile.is_open())
	{
		safe_close(m_hEvents[kDataEvent]);
		safe_close(m_hEvents[kExitEvent]);
		return false;
	}

	//中文支持（本地系统）
	std::wcout.imbue(std::locale(""));
	m_fsLogFile.imbue(std::locale(""));

	//启动工作线程
	m_bRun = true;
	m_pWorkerThread = new std::thread(WorkerThreadMain, this);
	if (nullptr == m_pWorkerThread)
	{
		Clear();
		return false;
	}
	
	return true;
}

//停止
void CTraceManager::Shutdown()
{
	if (!m_bRun)
	{
		return;
	}

	//通知工作线程结束
	m_bRun = false;
	::SetEvent(m_hEvents[kExitEvent]);

	//等待工作线程结束
	if (nullptr != m_pWorkerThread)
	{
		m_pWorkerThread->join();
	}
	safe_delete(m_pWorkerThread);
}

//添加日志信息(线程安全)
void CTraceManager::AppendMsg(const wchar* wsMsg)
{
	m_LogData.push(std::wstring(wsMsg));

	//置起数据事件
	::SetEvent(m_hEvents[TraceEvent::kDataEvent]);
}

//主逻辑刷新
void CTraceManager::Update()
{
	//等待事件触发
	DWORD dwRes = ::WaitForMultipleObjects(array_size(m_hEvents), m_hEvents, FALSE, INFINITE);
	if (dwRes >= WAIT_OBJECT_0 && dwRes < (WAIT_OBJECT_0 + array_size(m_hEvents)))
	{
		//打印日志
		int index = dwRes - WAIT_OBJECT_0;
		if (index == TraceEvent::kDataEvent)
		{
			std::wstring ws;
			while (m_LogData.pop(ws))
			{
				//文件保存
				m_fsLogFile << ws.c_str();
			}
		}
		//停止工作
		else if (index == TraceEvent::kExitEvent)
		{
			m_bRun = false;
		}
	}
	else
	{
		//等待错误
		m_bRun = false;
	}
}

//线程退出通知
void CTraceManager::OnExit()
{
	Clear();
}

//清理数据
void CTraceManager::Clear()
{
	m_bRun = false;
	for (size_t i = 0; i < array_size(m_hEvents); i++)
	{
		safe_close(m_hEvents[i]);
	}
	m_LogData.clear();
	if (m_fsLogFile.is_open())
	{
		m_fsLogFile.close();
	}
}
