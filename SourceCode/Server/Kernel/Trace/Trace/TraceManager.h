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

	//�����־��(�̰߳�ȫ)
	void AddLogItem(TraceLevel enLevel, COLORREF color, const wchar* wsMsg);

	//�����̵߳���
public:
	//���߼�ˢ��
	void Update();
	//�߳��˳�֪ͨ
	void OnExit();

	//�ڲ�����
private:
	//��־����
	struct LogItem
	{
		time_t time;			//��־ʱ��
		std::wstring wsMsg;		//��־����		
		TraceLevel enLevel;		//��־�ȼ�
		COLORREF	color;		//��ɫRGBֵ
		//���캯��
		LogItem() {}
		LogItem(time_t& tt, const wchar* ws, TraceLevel level, COLORREF cr) :
			time(tt), wsMsg(ws), enLevel(level), color(cr) {}
	};
	//��־���
	void PrintLog(LogItem& item);

	//��������
	void Clear();

	//��Ա����
private:
	//��־���ݻ���
	safe_queue<LogItem> m_LogData;

	//��ʱ���棬���Ч��
	LogItem m_LogTemp;

	//�����߳�
	std::atomic_bool m_bRun;
	std::thread* m_pWorkerThread;

	//�¼�֪ͨ
	HANDLE m_hEvents[TraceEvent::kNum];

	//��־��ʽ��������
	wchar m_wsBuffer[4096];

	//��־�ַ�������
	static wchar m_wsTraceLevel[TraceLevel::kTraceLevelNum][16];

	//��־�ļ�
	std::wofstream m_fsLogFile;
	std::wstring m_wsFileName;

private:
	//��ֹ��������ֵ
	DISALLOW_COPY_AND_ASSIGN(CTraceManager);
};

