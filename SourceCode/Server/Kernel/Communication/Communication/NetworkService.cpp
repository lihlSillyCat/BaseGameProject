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
#include "TCPServer.h"

CNetworkService::CNetworkService() :
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
		Trace()->LogErrorFormat(L"WSAStartup failed: %d\n", res);
		return false;
	}
	m_bRunning = true;

	return true;
}

//ֹͣ�������
void CNetworkService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}
	m_bRunning = false;

	for (auto itr = m_TCPServers.begin(); itr != m_TCPServers.end(); itr++)
	{
		(*itr)->Shutdown();
	}

	//����ϵͳ����
	WSACleanup();
}

//�ͷ���Դ
//���ú󲻵���ʹ�øö�����Ϊģ���ڲ��Ὣ������Դ�ͷš�
void CNetworkService::Release()
{
	for (auto itr = m_TCPServers.begin(); itr != m_TCPServers.end(); itr++)
	{
		(*itr)->Release();
		delete (*itr);
	}
}

//�������������
//������pConnectionHandler����������������пͻ������ӵ��¼�������
//������uPort��Χ [1024,49151], ����0��ʾϵͳ����ָ���˿�
//˵����Ŀǰ��֧��TCP
INetServer* CNetworkService::CreateServer(INetServerHandler* pServerHandler, ushort uPort, NetProtocol enProtocol, bool bEnableEnDecryption)
{
	if (!m_bRunning || nullptr == pServerHandler || (uPort != 0 && (uPort < 1024 || uPort > 49151)))
	{
		return nullptr;
	}

	switch (enProtocol)
	{	
	case NetProtocol::kTCP:
		return CreateTCPServer(pServerHandler, uPort, bEnableEnDecryption);
	default:
		break;
	}

	return nullptr;
}

//����TCP����˶���
INetServer* CNetworkService::CreateTCPServer(INetServerHandler* pServerHandler, ushort uPort, bool bEnableEnDecryption)
{
	CTCPServer* pServer = new CTCPServer(pServerHandler, bEnableEnDecryption);
	if (!pServer->Start(uPort))
	{
		return nullptr;
	}

	m_TCPServers.push_back(pServer);

	return pServer;
}
