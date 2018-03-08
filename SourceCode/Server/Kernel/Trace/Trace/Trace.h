#pragma once

#include "TraceManager.h"

#define LOG_BUFFER_SIZE	2048	//单条日志缓存大小

//日志颜色
enum TraceColor
{
	kLogColor = RGB(255, 255, 255),			//日志
	kDebugColor = RGB(255, 255, 255),		//DEBUG日志
	kInfoColor = RGB(255, 255, 255),		//信息
	kWarningColor = RGB(255, 255, 255),		//警告
	kAssertionColor = RGB(255, 255, 255),	//断言
	kErrorColor = RGB(255, 255, 255),		//错误
	kCriticalColor = RGB(255, 255, 255),	//临界点、关键危急等
	kExceptionColor = RGB(255, 255, 255),	//异常
	kCustomizeColor = RGB(255, 255, 255),	//用户自定义（默认值）
};

class CTrace : public ITraceService
{
public:
	explicit CTrace();
	~CTrace();

	//ITraceService 继承
public:
	//启动服务
	bool Start(ITraceView* pView, const wchar* wsLogFileName) override;
	//停止服务
	//停止后不得再使用该对象，因为函数内部会将本对象释放掉。
	void Shutdown() override;
	//服务状态
	bool Serviceable() override;

	//ITraceService::ITrace 继承
public:
	//日志输出
	void Log(TraceLevel enLevel, const wchar* wsMsg) override;
	void LogFormat(TraceLevel enLevel, const wchar* wsFormat, ...) override;

	//打印日志
	void Log(const wchar* wsMsg) override;
	void LogFormat(const wchar* wsFormat, ...) override;

	//打印DEBUG日志
	void LogDebug(const wchar* wsMsg) override;
	void LogDebugFormat(const wchar* wsFormat, ...) override;

	//打印信息
	void LogInfo(const wchar* wsMsg) override;
	void LogInfoFormat(const wchar* wsFormat, ...) override;

	//打印警告
	void LogWarning(const wchar* wsMsg) override;
	void LogWarningFormat(const wchar* wsFormat, ...) override;

	//打印断言
	void LogAssertion(const wchar* wsMsg) override;
	void LogAssertionFormat(const wchar* wsFormat, ...) override;

	//打印错误
	void LogError(const wchar* wsMsg) override;
	void LogErrorFormat(const wchar* wsFormat, ...) override;

	//打印关键危急
	virtual void LogCritical(const wchar* wsMsg) override;
	virtual void LogCriticalFormat(const wchar* wsFormat, ...) override;

	//打印异常
	void LogException(const wchar* wsMsg) override;
	void LogExceptionFormat(const wchar* wsFormat, ...) override;

	//打印自定义信息
	void LogCustom(ulong color, const wchar* wsMsg) override;
	void LogCustomFormat(ulong color, const wchar* wsFormat, ...) override;

	//代码定位
	void LogLocate(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsMsg) override;
	void LogLocateFormat(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsFormat, ...) override;

	//代码定位（__FILE__,__LINE__ 为char, 此处提供char版本）
	void LogLocate(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsMsg) override;
	void LogLocateFormat(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsFormat, ...) override;

	//功能函数
public:
	//获取颜色
	COLORREF GetColor(TraceLevel enLevel) const;
	//打印日志
	void PrintLog(TraceLevel enLevel, COLORREF color, const wchar* wsMsg);

	//内部成员
private:
	ITraceView* m_pView;
	CTraceManager m_TraceMgr;
	//数据解析缓存区
	wchar m_wsBuffer[LOG_BUFFER_SIZE];
	wchar m_wsPrintBuffer[LOG_BUFFER_SIZE];
	//日志字符串常量
	static wchar m_wsTraceLevel[TraceLevel::kTraceLevelNum][16];
	//读写锁，保证线程安全
	SRWLOCK m_SRWLock;
};
