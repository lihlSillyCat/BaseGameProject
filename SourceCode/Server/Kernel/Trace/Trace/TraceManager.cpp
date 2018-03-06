#include "stdafx.h"
#include <ctime>
#include "TraceManager.h"

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

//�����־��Ϣ(�̰߳�ȫ)
void CTraceManager::AppendMsg(const wchar* wsMsg)
{
	m_LogData.push(std::wstring(wsMsg));

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
			std::wstring ws;
			while (m_LogData.pop(ws))
			{
				//�ļ�����
				m_fsLogFile << ws.c_str();
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
