/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: ITrace.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/10/17
* Description:日志类

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/17		   1.0		  build this module
************************************************************************/
#pragma once
#include <assert.h>
#include "KernelModuleDef.h"

///////////////////////////////////////////////////////////////////////////////
//便利宏

//源代码定位
#define LOCATE_SC(trace, msg, type)	trace->LogLocate(type, __FILE__, __FUNCTION__, __LINE__, msg);

//断言（RELEASE版本输出日志，DEBUG版本输出日志且让程序直接崩掉）
#if defined(_DEBUG) || defined(DEBUG)
//断言判断
#define ASSERT_LOG(trace, expression, msg)	{ if (!(expression)) {LOCATE_SC(trace, msg, kTraceAssertion); assert(false);}} 
//断言错误
#define ASSERT_FALSE(trace, msg)	{LOCATE_SC(trace, msg, kTraceAssertion); assert(false);}
#else
//断言判断
#define ASSERT_LOG(trace, expression, msg)	{ if (!(expression)) LOCATE_SC(trace, msg, kTraceAssertion);} 
//断言错误
#define ASSERT_FALSE(trace, msg)	LOCATE_SC(trace, msg, kTraceAssertion)
#endif

//日志等级（从低到高）
enum TraceLevel
{
	kTraceLog,			//日志
	kTraceDebug,		//DEBUG日志
	kTraceInformation,	//信息
	kTraceWarning,		//警告
	kTraceAssertion,	//断言
	kTraceError,		//错误
	kTraceCritical,		//临界点、关键危急等
	kTraceException,	//异常
	kTraceCustomize,	//用户自定义
	kTraceLevelNum,		//等级数量（非用户使用）
};

//日志类接口(线程安全)
//只提供 UNICODE 版本 (计算机处理UNICODE字符串比ANSI字符串的效率高)
interface ITrace
{
	//日志输出
	virtual void Log(TraceLevel enLevel, const wchar* wsMsg) = 0;
	virtual void LogFormat(TraceLevel enLevel, const wchar* wsFormat, ...) = 0;

	//打印日志
	virtual void Log(const wchar* wsMsg) = 0;
	virtual void LogFormat(const wchar* wsFormat, ...) = 0;

	//打印DEBUG日志
	virtual void LogDebug(const wchar* wsMsg) = 0;
	virtual void LogDebugFormat(const wchar* wsFormat, ...) = 0;

	//打印信息
	virtual void LogInfo(const wchar* wsMsg) = 0;
	virtual void LogInfoFormat(const wchar* wsFormat, ...) = 0;

	//打印警告
	virtual void LogWarning(const wchar* wsMsg) = 0;
	virtual void LogWarningFormat(const wchar* wsFormat, ...) = 0;

	//打印断言
	virtual void LogAssertion(const wchar* wsMsg) = 0;
	virtual void LogAssertionFormat(const wchar* wsFormat, ...) = 0;

	//打印错误
	virtual void LogError(const wchar* wsMsg) = 0;
	virtual void LogErrorFormat(const wchar* wsFormat, ...) = 0;

	//打印关键危急
	virtual void LogCritical(const wchar* wsMsg) = 0;
	virtual void LogCriticalFormat(const wchar* wsFormat, ...) = 0;

	//打印异常
	virtual void LogException(const wchar* wsMsg) = 0;
	virtual void LogExceptionFormat(const wchar* wsFormat, ...) = 0;

	//打印自定义信息
	virtual void LogCustom(ulong color, const wchar* wsMsg) = 0;
	virtual void LogCustomFormat(ulong color, const wchar* wsFormat, ...) = 0;

	//代码定位
	virtual void LogLocate(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsMsg) = 0;
	virtual void LogLocateFormat(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsFormat, ...) = 0;
	//代码定位（__FILE__,__LINE__ 为char, 此处提供char版本）
	virtual void LogLocate(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsMsg) = 0;
	virtual void LogLocateFormat(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsFormat, ...) = 0;
};

//日志窗口视图
interface ITraceView
{
	//窗口输出
	virtual void PrintOnView(const wchar* wsMsg, ulong color = 0) = 0;
};

//日志服务类
interface ITraceService : public ITrace , public IService
{
	//启动服务
	//参数：pView界面视图
	//参数：wsLogFileName主日志文件名
	virtual bool Start(ITraceView* pView, const wchar* wsLogFileName) = 0;
};
