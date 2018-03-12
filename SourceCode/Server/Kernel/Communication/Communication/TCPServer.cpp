/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: TCPServer.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/5
* Description:TCP网络服务端

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
lihl		2018/3/1     	   1.1		  添加接收代码
************************************************************************/

#include "stdafx.h"
#include "TCPServer.h"
#include "NetConnection.h"

CTCPServer::CTCPServer(INetServerHandler* pServerHandler, bool bEnableEnDecryption) :
	m_pServerHandler(pServerHandler),
	m_fnAcceptEx(NULL),
	m_bEncryptionDecryption(bEnableEnDecryption)
{
	ASSERT_LOG(Trace(), pServerHandler != nullptr, L"网络服务器事件处理器不能为空");
}

CTCPServer::~CTCPServer()
{
}

//开始服务
bool CTCPServer::Start(ushort uPort)
{
	if (m_bRunning)
	{
		return true;
	}

	//创建服务Sock
	if (!CreateListenSocket(uPort))
	{
		return false;
	}

	//在主动器中注册本设备
	if (!Proactor()->RegisterDevice(this))
	{
		CloseSocket(m_socket);
		Trace()->LogError(L"注册设备失败！");
		return false;
	}

	if (!LoadfnAcceptExPtr())
	{
		return false;
	}

	//开始接收客户端网络连接
	if (!CreateClientsAndAccept())
	{
		Trace()->LogError(L"创建网络客户端连接并接收操作失败！");
		return false;
	}

	m_bRunning = true;	
	return true;
}

//初始化AcceptEx指针
bool CTCPServer::LoadfnAcceptExPtr()
{
	// Load the AcceptEx extension function from the provider for this socket
	DWORD bytes = 0;
	// GUID to Microsoft specific extensions
	GUID acceptex_guid = WSAID_ACCEPTEX;
	int nRet = WSAIoctl(
		m_socket,
		SIO_GET_EXTENSION_FUNCTION_POINTER,
		&acceptex_guid,
		sizeof(acceptex_guid),
		&m_fnAcceptEx,
		sizeof(m_fnAcceptEx),
		&bytes,
		NULL,
		NULL
	);
	if (nRet == SOCKET_ERROR)
	{
		Trace()->LogErrorFormat(L"failed to load AcceptEx: %d\n", WSAGetLastError());
		return false;
	}

	return true;
}

//停止服务
void CTCPServer::Shutdown()
{
	m_bRunning = false;

	for (auto itr = m_AcceptIOReqs.begin(); itr != m_AcceptIOReqs.end(); ++itr)
	{
		CloseSocket((*itr)->sockAccept);
	}

	CloseSocket(m_socket);
}

//清理资源
void CTCPServer::Release()
{
	for (auto itr = m_AcceptIOReqs.begin(); itr != m_AcceptIOReqs.end(); ++itr)
	{
		delete (*itr);
	}
}

//创建一个监听类型SOCKET
// Create a listening socket, bind, and set up its listening backlog.
bool CTCPServer::CreateListenSocket(ushort uPort)
{
	if (!CreateAsyncSock(m_socket, IPPROTO_TCP))
	{
		return false;
	}

	// Resolve the local address and port to be used by the server	
	addrinfo hints = {0};
	hints.ai_family = AF_INET;	//IPv4协议族
	hints.ai_socktype = SOCK_STREAM; //socket流
	hints.ai_protocol = IPPROTO_TCP; //TCP网络协议
	hints.ai_flags = AI_PASSIVE; //指示绑定操作

	char port[32] = {0};
	sprintf_s(port, "%d", uPort);
	addrinfo *addrlocal = nullptr;
	sint res = getaddrinfo(NULL, port, &hints, &addrlocal);
	if (0 != res || nullptr == addrlocal)
	{
		Trace()->LogErrorFormat(L"getaddrinfo failed[res:%d][error:%d]", res, WSAGetLastError());
		return false;
	}

	//绑定端口
	res = bind(m_socket, addrlocal->ai_addr, static_cast<int>(addrlocal->ai_addrlen));
	if (SOCKET_ERROR == res)
	{
		Trace()->LogErrorFormat(L"bind failed with error:%d", WSAGetLastError());
		freeaddrinfo(addrlocal);
		CloseSocket(m_socket);
		return false;
	}

	//绑定完成后，释放地址信息
	m_sockaddr = *((sockaddr_in*)addrlocal->ai_addr);
	freeaddrinfo(addrlocal);
	addrlocal = nullptr;

	//监听SOCKET
	if (SOCKET_ERROR == listen(m_socket, SOMAXCONN))
	{
		Trace()->LogErrorFormat(L"listen failed with error:%d", WSAGetLastError());
		CloseSocket(m_socket);
		return false;
	}

	return true;
}

//地址信息(ip和端口)
void CTCPServer::GetLocalAddr(ulong& ip, ushort& port)
{
	ip = m_sockaddr.sin_addr.S_un.S_addr;
	port = m_sockaddr.sin_port;
}

void CTCPServer::GetLocalAddr(wchar* wsIP, uint nSize, ushort& port)
{
	port = m_sockaddr.sin_port;
	InetNtop(m_sockaddr.sin_family, &m_sockaddr.sin_addr, wsIP, nSize);
}

//创建客户端连接并用于Accept
bool CTCPServer::CreateClientsAndAccept()
{
	//当容器中数量过小则批量创建
	if (m_AcceptIOReqs.size() >= NetConstant::kIncomingConnectionMinNum)
	{
		return true;
	}

	DWORD dwBytesReceived;
	for (size_t i = m_AcceptIOReqs.size(); i < NetConstant::kIncomingConnectionMaxNum; i++)
	{
		CNetAcceptIOReq* pIORequst = new CNetAcceptIOReq();
		CreateAsyncSock(pIORequst->sockAccept, IPPROTO_TCP);

		//接收客户端连接
		//将dwReceiveDataLength设置为0，当连接到达时AcceptEx直接返回成功，不等待接收数据。
		if (FALSE == m_fnAcceptEx(m_socket, pIORequst->sockAccept,
			pIORequst->sDataBuf, 0, NetConstant::kAddrBufSize, NetConstant::kAddrBufSize,
			&dwBytesReceived, pIORequst))
		{
			int error = WSAGetLastError();
			if (ERROR_IO_PENDING != error)
			{
				Trace()->LogErrorFormat(L"AcceptEx fails with error[%d]", error);
				delete pIORequst;
				continue;
			}
		}
		m_AcceptIOReqs.push_back(pIORequst);
	}

	return m_AcceptIOReqs.size() > NetConstant::kIncomingConnectionMinNum;
}

//操作完成通知
//参数 pIORequst：IO请求包
void CTCPServer::OnIOCompleted(IIORequst* pIORequst)
{	
	switch (((CNetIORequst*)pIORequst)->NetOperation)
	{
	case NetAsyncOperation::kAccept:
		OnIOCompletedAccept((CNetAcceptIOReq*)pIORequst);
		break;
	default:
		break;
	}
}

//操作完成通知（失败）
//参数 pIORequst：IO请求包
//参数 nErrorCode ： 错误码
void CTCPServer::OnIOCompletedError(IIORequst* pIORequst, ulong nErrorCode)
{
	switch (((CNetIORequst*)pIORequst)->NetOperation)
	{
	case NetAsyncOperation::kAccept:
		OnIOCompletedAcceptError((CNetAcceptIOReq*)pIORequst, nErrorCode);
		break;
	default:
		break;
	}
}

//接收到新的连接
void CTCPServer::OnIOCompletedAccept(CNetAcceptIOReq* pIORequst)
{
	//设置属性
	if(SOCKET_ERROR == setsockopt(pIORequst->sockAccept, SOL_SOCKET, 
		SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_socket, sizeof(m_socket)))
	{
		Trace()->LogWarningFormat(L"setsockopt(SO_UPDATE_ACCEPT_CONTEXT) failed to update accept socket. ErrorCode[%d]", WSAGetLastError());
		delete pIORequst;
		return;
	}

	//提取远程连接的地址
	sockaddr_in localaddr, remoteaddr;
	sint localaddrlen, remoteaddrlen;
	GetAcceptExSockaddrs(pIORequst->sDataBuf, 0, NetConstant::kAddrBufSize, NetConstant::kAddrBufSize,
		(sockaddr**)&localaddr, &localaddrlen, (sockaddr**)&remoteaddr, &remoteaddrlen);

	//生成一新的连接并释放IO请求包
	CNetConnection* pConnection = NetConnectionPool()->NewObject();
	pConnection->Init(pIORequst->sockAccept, remoteaddr, m_pConnectionHandler, m_bEncryptionDecryption);
	auto itr = std::find(m_AcceptIOReqs.begin(), m_AcceptIOReqs.end(), pIORequst);
	if (itr != m_AcceptIOReqs.end())
	{
		m_AcceptIOReqs.erase(itr);
	}
	delete pIORequst;

	//将新连接加入主动器
	Proactor()->RegisterDevice(pConnection);

	//通知调度器
	m_pServerHandler->OnAccept(pConnection);

	//开始工作
	pConnection->Start();

	//继续接收
	CreateClientsAndAccept();
}

//操作完成通知（失败）
void CTCPServer::OnIOCompletedAcceptError(CNetAcceptIOReq* pIORequst, ulong nErrorCode)
{
	m_pServerHandler->OnError(nErrorCode);

	auto itr = std::find(m_AcceptIOReqs.begin(), m_AcceptIOReqs.end(), pIORequst);
	if (itr != m_AcceptIOReqs.end())
	{
		m_AcceptIOReqs.erase(itr);
	}
	delete pIORequst;
}
