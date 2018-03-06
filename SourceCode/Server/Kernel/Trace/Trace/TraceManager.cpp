#include "stdafx.h"
#include <ctime>
#include "TraceManager.h"

//��־�ַ�������
wchar CTraceManager::m_wsTraceLevel[TraceLevel::kTraceLevelNum][16] = 
{
	L"Log",			//��־
	L"Debug",		//DEBUG��־
	L"Infor",		//��Ϣ
	L"Warning",		//����
	L"Assertion",	//����
	L"Error",		//����
	L"Critical",	//�ٽ�㡢�ؼ�Σ����
	L"Exception",	//�쳣
	L"Customize",	//�û��Զ���
};

//�����߳�
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

//��־������
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

//��ʼ
bool CTraceManager::Start()
{
	if (m_bRun)
	{
		return true;
	}

	//���������¼����˳��¼�
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

	//���ļ�
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

	//����֧�֣�����ϵͳ��
	std::wcout.imbue(std::locale(""));
	m_fsLogFile.imbue(std::locale(""));

	//���������߳�
	m_bRun = true;
	m_pWorkerThread = new std::thread(WorkerThreadMain, this);
	if (nullptr == m_pWorkerThread)
	{
		Clear();
		return false;
	}
	
	return true;
}

//ֹͣ
void CTraceManager::Shutdown()
{
	if (!m_bRun)
	{
		return;
	}

	//֪ͨ�����߳̽���
	m_bRun = false;
	::SetEvent(m_hEvents[kExitEvent]);

	//�ȴ������߳̽���
	if (nullptr != m_pWorkerThread)
	{
		m_pWorkerThread->join();
	}
	safe_delete(m_pWorkerThread);
}

//�����־��
void CTraceManager::AddLogItem(TraceLevel enLevel, COLORREF color, const wchar* wsMsg)
{
	if (!m_bRun || nullptr == wsMsg)
	{
		return;
	}

	//�������
	time_t tt = time(nullptr);
	m_LogData.push(LogItem(tt, wsMsg, enLevel, color));

	//���������¼�
	::SetEvent(m_hEvents[TraceEvent::kDataEvent]);
}

//���߼�ˢ��
void CTraceManager::Update()
{
	//�ȴ��¼�����
	DWORD dwRes = ::WaitForMultipleObjects(array_size(m_hEvents), m_hEvents, FALSE, INFINITE);
	if (dwRes >= WAIT_OBJECT_0 && dwRes < (WAIT_OBJECT_0 + array_size(m_hEvents)))
	{
		//��ӡ��־
		int index = dwRes - WAIT_OBJECT_0;
		if (index == TraceEvent::kDataEvent)
		{
			while (m_LogData.pop(m_LogTemp))
			{
				PrintLog(m_LogTemp);
			}
		}
		//ֹͣ����
		else if (index == TraceEvent::kExitEvent)
		{
			m_bRun = false;
		}
	}
	else
	{
		//�ȴ�����
		m_bRun = false;
	}
}

//��־���
void CTraceManager::PrintLog(LogItem& item)
{
	//��ʽ����־�ַ���
	tm t;
	localtime_s(&t, &item.time);

	_snwprintf_s(m_wsBuffer, array_size(m_wsBuffer), L"[%04d-%02d-%02d %02d:%02d:%02d][%s]:%s\r\n",
		t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec,
		m_wsTraceLevel[item.enLevel], item.wsMsg.c_str());

	////��Ļ���
	//std::wcout << m_wsBuffer << std::endl;

	////�ļ�����
	//m_fsLogFile << m_wsBuffer << std::endl;

	//Note:ֱ��ʹ�� \r\n ���б� std::endl Ҫ�á�
	//��Ļ���
	std::wcout << m_wsBuffer;

	//�ļ�����
	m_fsLogFile << m_wsBuffer;
}

//�߳��˳�֪ͨ
void CTraceManager::OnExit()
{
	Clear();
}

//��������
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
