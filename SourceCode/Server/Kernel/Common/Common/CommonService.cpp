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
	if (nullptr == pTrace || m_bRunning)
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
}

//�ͷ���Դ
//���ú󲻵���ʹ�øö�����Ϊģ���ڲ��Ὣ������Դ�ͷš�
void CCommonService::Release()
{
	for (auto itr = m_Proactors.begin(); itr != m_Proactors.end(); itr++)
	{
		delete (*itr);
	}

	CSharedRes::Instance()->Release();

	delete this;
}

//��ȡһ��������
//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
IProactor* CCommonService::CreateProactor(sint nConcurrentThreads)
{
	if (!m_bRunning)
	{
		return nullptr;
	}

	CProactor* pProactor = new CProactor();
	if (!pProactor->Start(nConcurrentThreads))
	{
		delete pProactor;
		return nullptr;
	}

	m_Proactors.push_back(pProactor);
	return pProactor;
}
