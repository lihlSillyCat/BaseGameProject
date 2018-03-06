#pragma once
#include <atomic>
#include <thread>
#include <fstream>
#include "Singleton.h"
#include "SafeContainer.h"

//��־�¼�
enum TraceEvent
{
	kDataEvent,	//�����¼�
	kExitEvent,	//�˳��¼�
	kNum,		//�¼�����
};

class CTraceManager
{
public:
	CTraceManager(wchar* wsName);
	~CTraceManager();
	
	//���ܺ���
public:
	//��ʼ
	bool Start();
	//ֹͣ
	void Shutdown();
	//��ת��
	bool Running() const { return m_bRun; }
	//�����־��Ϣ(�̰߳�ȫ)
	void AppendMsg(const wchar* wsMsg);

	//�����̵߳���
public:
	//���߼�ˢ��
	void Update();
	//�߳��˳�֪ͨ
	void OnExit();

	//�ڲ�����
private:
	//��������
	void Clear();

	//��Ա����
private:
	//��־���ݻ���
	safe_queue<std::wstring> m_LogData;

	//�����߳�
	std::atomic_bool m_bRun;
	std::thread* m_pWorkerThread;

	//�¼�֪ͨ
	HANDLE m_hEvents[TraceEvent::kNum];

	//��־�ļ�
	std::wofstream m_fsLogFile;
	std::wstring m_wsFileName;

private:
	//��ֹ��������ֵ
	DISALLOW_COPY_AND_ASSIGN(CTraceManager);
};

