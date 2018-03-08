/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: CommunicationService.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/7
* Description:提供通信服务（网络通信和进程通信）

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/7    	   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "CommunicationService.h"


CCommunicationService::CCommunicationService():
	m_bRunning(false)
{
	m_Version.major = 1;
	m_Version.minor = 0;
	m_Version.revision = 1;
	wcscpy_s(m_Version.build, L"dev 1.0");
}

CCommunicationService::~CCommunicationService()
{
}

//开始网络服务
bool CCommunicationService::Start(ITrace* pTrace, IProactor* pProactor, DevelopmentEnvironment DevEnvironment)
{
	if (nullptr == pTrace || nullptr == pProactor || m_bRunning)
	{
		return false;
	}

	CSharedRes::Instance()->SetTrace(pTrace);
	CSharedRes::Instance()->SetProactor(pProactor);
	CSharedRes::Instance()->SetDevEnvironment(DevEnvironment);

	m_bRunning = true;
	return true;
}

//停止网络服务
//注意：调用本函数后不得再使用该对象。应将指针置为空。
//如果需要再次使用网络通信服务，则需重新创建对象
void CCommunicationService::Shutdown()
{
	m_bRunning = false;
	m_NetworkService.Shutdown();

	CSharedRes::Instance()->Release();

	delete this;
}

//获取网络服务
//失败返回 nullptr; 成功则返回已启动成功的网络服务对象
INetworkService* CCommunicationService::GetNetworkService()
{
	if (!m_bRunning)
	{
		return nullptr;
	}

	if (m_NetworkService.Serviceable())
	{
		return static_cast<INetworkService*>(&m_NetworkService);
	}

	if (!m_NetworkService.Start())
	{
		return nullptr;
	}

	return static_cast<INetworkService*>(&m_NetworkService);
}
