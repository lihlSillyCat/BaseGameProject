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

	//添加日志项(线程安全)
	void AddLogItem(TraceLevel enLevel, COLORREF color, const wchar* wsMsg);

	//供给线程调度
public:
	//主逻辑刷新
	void Update();
	//线程退出通知
	void OnExit();

	//内部函数
private:
	//日志数据
	struct LogItem
	{
		time_t time;			//日志时间
		std::wstring wsMsg;		//日志内容		
		TraceLevel enLevel;		//日志等级
		COLORREF	color;		//颜色RGB值
		//构造函数
		LogItem() {}
		LogItem(time_t& tt, const wchar* ws, TraceLevel level, COLORREF cr) :
			time(tt), wsMsg(ws), enLevel(level), color(cr) {}
	};
	//日志输出
	void PrintLog(LogItem& item);

	//清理数据
	void Clear();

	//成员变量
private:
	//日志数据缓存
	safe_queue<LogItem> m_LogData;

	//临时缓存，提高效率
	LogItem m_LogTemp;

	//工作线程
	std::atomic_bool m_bRun;
	std::thread* m_pWorkerThread;

	//事件通知
	HANDLE m_hEvents[TraceEvent::kNum];

	//日志格式化缓冲区
	wchar m_wsBuffer[4096];

	//日志字符串常量
	static wchar m_wsTraceLevel[TraceLevel::kTraceLevelNum][16];

	//日志文件
	std::wofstream m_fsLogFile;
	std::wstring m_wsFileName;

private:
	//禁止拷贝、赋值
	DISALLOW_COPY_AND_ASSIGN(CTraceManager);
};

