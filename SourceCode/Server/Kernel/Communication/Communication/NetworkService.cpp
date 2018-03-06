/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: NetworkService.cpp
* Author: �����
* Version: 1.0
* Date: 2018/2/5
* Description:����ͨ�ŷ���

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "NetworkService.h"


CNetworkService::CNetworkService(ITrace* pTrace) :
	m_pTrace(pTrace),
	m_bRunning(false)
{

}

CNetworkService::~CNetworkService()
{

}

//��ʼ�������
bool CNetworkService::Start()
{
	if (m_bRunning)
	{
		return true;
	}

	// Initialize Winsock
	int res = WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (res != 0)
	{
		m_pTrace->LogErrorFormat(L"WSAStartup failed: %d\n", res);
		return false;
	}
	m_bRunning = true;

	return true;
}

//ֹͣ�������
//ע�⣺���ñ������󲻵���ʹ�øö���Ӧ��ָ����Ϊ�ա�
//�����Ҫ�ٴ�ʹ������ͨ�ŷ����������´�������
void CNetworkService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}
	m_bRunning = false;

	//����ϵͳ����
	WSACleanup();

	//���ͷ�
	delete this;
}

//�ɷ���״̬
bool CNetworkService::Serviceable()
{
	return m_bRunning;
}
