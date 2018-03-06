#include "stdafx.h"
#include "CommonService.h"
#include "Proactor.h"

CCommonService::CCommonService():
	m_bRunning(false)
{

}

CCommonService::~CCommonService()
{

}

//��������
bool CCommonService::Start(ITrace* pTrace, DevelopmentEnvironment DevEnvironment)
{
	if (nullptr == pTrace)
	{
		return false;
	}

	CSharedRes::Instance()->SetTrace(pTrace);
	CSharedRes::Instance()->SetDevEnvironment(DevEnvironment);	

	m_bRunning = true;

	return true;
}

//ֹͣ����
//ֹͣ�󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
void CCommonService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}

	m_bRunning = false;

	for (auto itr = m_Proactors.begin(); itr != m_Proactors.end(); itr++)
	{
		(*itr)->Shutdown();
	}

	CSharedRes::Instance()->Release();

	delete this;
}

//����״̬
bool CCommonService::Serviceable()
{
	return m_bRunning;
}

//��ȡһ��������
//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
IProactor* CCommonService::CreateProactor(sint nConcurrentThreads)
{
	if (!m_bRunning)
	{
		return nullptr;
	}

	IProactorService* pProactor = new CProactor();
	if (!pProactor->Start(nConcurrentThreads))
	{
		delete pProactor;
		return nullptr;
	}

	m_Proactors.push_back(pProactor);
	return pProactor;
}
