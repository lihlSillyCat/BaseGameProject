#include "stdafx.h"
#include "SharedRes.h"


CSharedRes::CSharedRes()
{
}


CSharedRes::~CSharedRes()
{
}

//��־
void CSharedRes::SetTrace(ITrace* pTrace)
{
	m_pTrace = pTrace;
}

//������
void CSharedRes::SetProactor(IProactor* pProactor)
{
	m_pProactor = pProactor;
}

//Ӧ�ó�����Ϣ
void CSharedRes::SetAppInfo(IAppInfo* pAppInfo)
{
	m_pAppInfo = pAppInfo;
}

