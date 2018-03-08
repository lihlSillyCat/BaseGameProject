/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: CommunicationService.cpp
* Author: �����
* Version: 1.0
* Date: 2018/3/7
* Description:�ṩͨ�ŷ�������ͨ�źͽ���ͨ�ţ�

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

//��ʼ�������
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

//ֹͣ�������
//ע�⣺���ñ������󲻵���ʹ�øö���Ӧ��ָ����Ϊ�ա�
//�����Ҫ�ٴ�ʹ������ͨ�ŷ����������´�������
void CCommunicationService::Shutdown()
{
	m_bRunning = false;
	m_NetworkService.Shutdown();

	CSharedRes::Instance()->Release();

	delete this;
}

//��ȡ�������
//ʧ�ܷ��� nullptr; �ɹ��򷵻��������ɹ�������������
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
