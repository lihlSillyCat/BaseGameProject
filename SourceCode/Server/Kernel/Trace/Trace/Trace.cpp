// Trace.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Trace.h"

//日志字符串常量
wchar CTrace::m_wsTraceLevel[TraceLevel::kTraceLevelNum][16] =
{
	L"Log",			//日志
	L"Debug",		//DEBUG日志
	L"Infor",		//信息
	L"Warning",		//警告
	L"Assertion",	//断言
	L"Error",		//错误
	L"Critical",	//临界点、关键危急等
	L"Exception",	//异常
	L"Customize",	//用户自定义
};

//日志服务类
CTrace::CTrace():
	m_pView(nullptr)
{
	::InitializeSRWLock(&m_SRWLock);
}

CTrace::~CTrace()
{

}

//启动服务
bool CTrace::Start(ITraceView* pView, const wchar* wsLogFileName)
{
	if (m_TraceMgr.Running())
	{
		return true;
	}

	assert(nullptr != pView);
	m_pView = pView;
	if (!m_TraceMgr.Start(wsLogFileName))
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
}

//服务状态
bool CTrace::Serviceable()
{
	return m_TraceMgr.Running();
}

//释放资源
//调用后不得再使用该对象，因为模块内部会将所有资源释放。
void CTrace::Release()
{
	delete this;
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

	PrintLog(enLevel, GetColor(enLevel), m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印日志
void CTrace::Log(const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceLog, TraceColor::kLogColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);
	
	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceLog, TraceColor::kLogColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印DEBUG日志
void CTrace::LogDebug(const wchar* wsMsg)
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceDebug, TraceColor::kDebugColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
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

	PrintLog(TraceLevel::kTraceDebug, TraceColor::kDebugColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印信息
void CTrace::LogInfo(const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceInformation, TraceColor::kInfoColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogInfoFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceInformation, TraceColor::kInfoColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印警告
void CTrace::LogWarning(const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceWarning, TraceColor::kWarningColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogWarningFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceWarning, TraceColor::kWarningColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印断言
void CTrace::LogAssertion(const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceAssertion, TraceColor::kAssertionColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogAssertionFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceAssertion, TraceColor::kAssertionColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印错误
void CTrace::LogError(const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceError, TraceColor::kErrorColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogErrorFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceError, TraceColor::kErrorColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印关键危急
void CTrace::LogCritical(const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceCritical, TraceColor::kCriticalColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogCriticalFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceCritical, TraceColor::kCriticalColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印异常
void CTrace::LogException(const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceException, TraceColor::kExceptionColor, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogExceptionFormat(const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceException, TraceColor::kExceptionColor, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//打印自定义信息
void CTrace::LogCustom(ulong color, const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	PrintLog(TraceLevel::kTraceCustomize, color, wsMsg);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

void CTrace::LogCustomFormat(ulong color, const wchar* wsFormat, ...)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	va_list args;
	va_start(args, wsFormat);
	_vsnwprintf_s(m_wsBuffer, array_size(m_wsBuffer) - 1, wsFormat, args);
	va_end(args);

	PrintLog(TraceLevel::kTraceCustomize, color, m_wsBuffer);

	::ReleaseSRWLockExclusive(&m_SRWLock);
}

//代码定位
void CTrace::LogLocate(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsMsg)
{
	//读写锁，独占资源
	::AcquireSRWLockExclusive(&m_SRWLock);

	_snwprintf_s(m_wsBuffer, array_size(m_wsBuffer), L"SrcCode[%s,%s,line:%d]Msg[%s]",
		wsFileName, wsFuncName, uLineCnt, wsMsg);
	PrintLog(enLevel, GetColor(enLevel), m_wsBuffer);

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
	PrintLog(enLevel, GetColor(enLevel), m_wsBuffer);

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
	PrintLog(enLevel, GetColor(enLevel), m_wsBuffer);

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
	PrintLog(enLevel, GetColor(enLevel), m_wsBuffer);

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

//打印日志
void CTrace::PrintLog(TraceLevel enLevel, COLORREF color, const wchar* wsMsg)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	_snwprintf_s(m_wsPrintBuffer, array_size(m_wsPrintBuffer), L"[%04d-%02d-%02d %02d:%02d:%02d,%03d][%s]%s\r\n",
		st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
		m_wsTraceLevel[enLevel], wsMsg);

	//屏幕视图输出
	m_pView->PrintOnView(m_wsPrintBuffer, color);

	//其他方式输出（文件、第三方后台等）
	m_TraceMgr.AppendMsg(m_wsPrintBuffer);
}
