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

//启动服务
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

//停止服务
//停止后不得再使用该对象，因为函数内部会将本对象释放掉。
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

//服务状态
bool CCommonService::Serviceable()
{
	return m_bRunning;
}

//获取一个主动器
//参数 nConcurrentThreads：并发线程数。不建议用户填写该参数。若无特殊要求则使用默认值
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
