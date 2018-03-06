#pragma once
#include <atomic>
#include <thread>
#include <fstream>
#include "Singleton.h"
#include "SafeContainer.h"

//日志事件
enum TraceEvent
{
	kDataEvent,	//数据事件
	kExitEvent,	//退出事件
	kNum,		//事件数量
};

class CTraceManager
{
public:
	CTraceManager(wchar* wsName);
	~CTraceManager();
	
	//功能函数
public:
	//开始
	bool Start();
	//停止
	void Shutdown();
	//运转中
	bool Running() const { return m_bRun; }
	//添加日志信息(线程安全)
	void AppendMsg(const wchar* wsMsg);

	//供给线程调度
public:
	//主逻辑刷新
	void Update();
	//线程退出通知
	void OnExit();

	//内部函数
private:
	//清理数据
	void Clear();

	//成员变量
private:
	//日志数据缓存
	safe_queue<std::wstring> m_LogData;

	//工作线程
	std::atomic_bool m_bRun;
	std::thread* m_pWorkerThread;

	//事件通知
	HANDLE m_hEvents[TraceEvent::kNum];

	//日志文件
	std::wofstream m_fsLogFile;
	std::wstring m_wsFileName;

private:
	//禁止拷贝、赋值
	DISALLOW_COPY_AND_ASSIGN(CTraceManager);
};

