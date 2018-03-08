/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: NetworkService.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/5
* Description:网络通信服务

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

//开始网络服务
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

//停止网络服务
//注意：调用本函数后不得再使用该对象。应将指针置为空。
//如果需要再次使用网络通信服务，则需重新创建对象
void CNetworkService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}
	m_bRunning = false;

	for (auto itr = m_vecTCPServers.begin(); itr != m_vecTCPServers.end(); itr++)
	{
		(*itr)->Shutdown();
		delete (*itr);
	}

	//清理系统网络
	WSACleanup();
}

//可服务状态
bool CNetworkService::Serviceable()
{
	return m_bRunning;
}

//创建网络服务器
//参数：pConnectionHandler该网络服务器下所有客户端连接的事件处理器
//参数：uPort范围 [1024,49151], 传入0表示系统任意指定端口
//说明：目前仅支持TCP
INetServer* CNetworkService::CreateServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
	ushort uPort, NetProtocol enProtocol, bool bEnableEnDecryption)
{
	if (!m_bRunning || nullptr == pServerHandler || nullptr == pConnectionHandler 
		|| (uPort != 0 && (uPort < 1024 || uPort > 49151)))
	{
		return nullptr;
	}

	switch (enProtocol)
	{	
	case NetProtocol::kTCP:
		return CreateTCPServer(pServerHandler, pConnectionHandler, uPort, bEnableEnDecryption);
	default:
		break;
	}

	return nullptr;
}

//创建TCP服务端对象
INetServer* CNetworkService::CreateTCPServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
	ushort uPort, bool bEnableEnDecryption)
{
	CTCPServer* pServer = new CTCPServer(pServerHandler, pConnectionHandler, bEnableEnDecryption);
	if (!pServer->Start(uPort))
	{
		return nullptr;
	}

	m_vecTCPServers.push_back(pServer);

	return pServer;
}
