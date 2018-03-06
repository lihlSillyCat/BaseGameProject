// Trace.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Trace.h"

//日志服务类
CTrace::CTrace(wchar* wsName, ITraceSink* pSink):
	m_pSink(pSink),
	m_TraceMgr(wsName)
{
	::InitializeSRWLock(&m_SRWLock);
}

CTrace::~CTrace()
{

}

//启动服务
bool CTrace::Start()
{
	if (m_TraceMgr.Running())
	{
		return true;
	}

	if (!m_TraceMgr.Start())
	{
		return false;
	}
	
	return true;
}

//停止服务
//停止后不得再使用该对象，因为函数内部会将本对象释放掉。
void CTrace::Shutdown()
{
	m_TraceMgr.Shutdown();
	delete this;
}

//服务状态
bool CTrace::Serviceable()
{
	return m_TraceMgr.Running();
}

//日志输出
void CTrace::Log(TraceLevel enLevel, const wchar* wsMsg)
{
	switch (enLevel)
	{
	case kTraceLog:
		Log(wsMsg);
		break;
	case kTraceDebug:
		LogDebug(wsMsg);
		break;
	case kTraceInformation:
		LogInfo(wsMsg);
		break;
	case kTraceWarning:
		LogWarning(wsMsg);
		break;
	case kTraceAssertion:
		LogAssertion(wsMsg);
		break;
	case kTraceError:
		LogError(wsMsg);
		break;
	case kTraceCritical:
		LogCritical(wsMsg);
		break;
	case kTraceException:
		LogException(wsMsg);
		break;
	case kTraceCustomize:
		LogCustom(0, wsMsg);
		break;
	default:
		break;
	}
}

void CTrace::LogFormat(TraceLevel enLevel, const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(enLevel, GetColor(enLevel), m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印日志
void CTrace::Log(const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceLog, TraceColor::kLogColor, wsMsg);
}

void CTrace::LogFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);
	
	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceLog, TraceColor::kLogColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印DEBUG日志
void CTrace::LogDebug(const wchar* wsMsg)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	m_TraceMgr.AddLogItem(TraceLevel::kTraceDebug, TraceColor::kDebugColor, wsMsg);
}

void CTrace::LogDebugFormat(const wchar* wsFormat, ...)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceDebug, TraceColor::kDebugColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印信息
void CTrace::LogInfo(const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceInformation, TraceColor::kInfoColor, wsMsg);
}

void CTrace::LogInfoFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceInformation, TraceColor::kInfoColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印警告
void CTrace::LogWarning(const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceWarning, TraceColor::kWarningColor, wsMsg);
}

void CTrace::LogWarningFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceWarning, TraceColor::kWarningColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印断言
void CTrace::LogAssertion(const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceAssertion, TraceColor::kAssertionColor, wsMsg);
}

void CTrace::LogAssertionFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceAssertion, TraceColor::kAssertionColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印错误
void CTrace::LogError(const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceError, TraceColor::kErrorColor, wsMsg);
}

void CTrace::LogErrorFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceError, TraceColor::kErrorColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印关键危急
void CTrace::LogCritical(const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceCritical, TraceColor::kCriticalColor, wsMsg);
}

void CTrace::LogCriticalFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceCritical, TraceColor::kCriticalColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印异常
void CTrace::LogException(const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceException, TraceColor::kExceptionColor, wsMsg);
}

void CTrace::LogExceptionFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceException, TraceColor::kExceptionColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印自定义信息
void CTrace::LogCustom(COLORREF color, const wchar* wsMsg)
{
	m_TraceMgr.AddLogItem(TraceLevel::kTraceCustomize, color, wsMsg);
}

void CTrace::LogCustomFormat(COLORREF color, const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	m_TraceMgr.AddLogItem(TraceLevel::kTraceCustomize, color, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//代码定位
void CTrace::LogLocate(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	_snwprintf_s(m_wsBuffer, array_size(m_wsBuffer), L"SrcCode[%s,%s,line:%d]Msg[%s]",
		wsFileName, wsFuncName, uLineCnt, wsMsg);
	m_TraceMgr.AddLogItem(enLevel, GetColor(enLevel), m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogLocateFormat(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	wchar buffer[2048];
	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(buffer, array_size(buffer) - 1, wsFormat, args);
	va_end(args);

	_snwprintf_s(m_wsBuffer, array_size(m_wsBuffer), L"SrcCode[%s,%s,line:%d]Msg[%s]",
		wsFileName, wsFuncName, uLineCnt, buffer);
	m_TraceMgr.AddLogItem(enLevel, GetColor(enLevel), m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//代码定位（__FILE__,__LINE__ 为char, 此处提供char版本）
void CTrace::LogLocate(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	//"%hs" 用于转换char; %ls 用于转换 wchar
	_snwprintf_s(m_wsBuffer, array_size(m_wsBuffer), L"SrcCode[%hs,%hs,line:%d]Msg[%ls]",
		strFileName, strFuncName, uLineCnt, wsMsg);
	m_TraceMgr.AddLogItem(enLevel, GetColor(enLevel), m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogLocateFormat(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	wchar buffer[2048];
	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(buffer, array_size(buffer) - 1, wsFormat, args);
	va_end(args);

	//"%hs" 用于转换char; %ls 用于转换 wchar
	_snwprintf_s(m_wsBuffer, array_size(m_wsBuffer), L"SrcCode[%hs,%hs,line:%d]Msg[%ls]",
		strFileName, strFuncName, uLineCnt, buffer);
	m_TraceMgr.AddLogItem(enLevel, GetColor(enLevel), m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//获取颜色
COLORREF CTrace::GetColor(TraceLevel enLevel) const
{
	switch (enLevel)
	{
	case kTraceLog:
		return TraceColor::kLogColor;
	case kTraceDebug:
		return TraceColor::kDebugColor;
	case kTraceInformation:
		return TraceColor::kInfoColor;
	case kTraceWarning:
		return TraceColor::kWarningColor;
	case kTraceAssertion:
		return TraceColor::kAssertionColor;
	case kTraceError:
		return TraceColor::kErrorColor;
	case kTraceCritical:
		return TraceColor::kCriticalColor;
	case kTraceException:
		return TraceColor::kExceptionColor;
	case kTraceCustomize:
		return TraceColor::kCustomizeColor;
	default:
		return TraceColor::kLogColor;
	}
}
