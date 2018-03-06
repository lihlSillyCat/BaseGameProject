#include "stdafx.h"
#include "SharedRes.h"


CSharedRes::CSharedRes()
{
}


CSharedRes::~CSharedRes()
{
}

//日志
void CSharedRes::SetTrace(ITrace* pTrace)
{
	m_pTrace = pTrace;
}

//主动器
void CSharedRes::SetProactor(IProactor* pProactor)
{
	m_pProactor = pProactor;
}

//应用程序信息
void CSharedRes::SetAppInfo(IAppInfo* pAppInfo)
{
	m_pAppInfo = pAppInfo;
}

