/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: Winsock.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2018/1/5
* Description: Windows Socket 2 服务，所有网络设备的基类

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/1/5     	   1.0		  build this module
lihl		2018/2/22     	   1.1		  完善一些事项
************************************************************************/

#include "stdafx.h"
#include "Winsock.h"

CWinsock::CWinsock():
	m_bRunning(false),
	m_wsName(L"网络设备")
{

}

CWinsock::~CWinsock()
{

}

//创建异步套接字
// Create a socket with all the socket options we need, namely disable buffering and set linger.
bool CWinsock::CreateAsyncSock(SOCKET &Sock, sint nProtocol)
{
	//创建异步SOCKET
	Sock = WSASocket(AF_INET, SOCK_STREAM, nProtocol, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (Sock == INVALID_SOCKET)
	{
		Trace()->LogErrorFormat(L"WSASocket failed: %d", WSAGetLastError());
		return false;
	}

	//
	// Disable send buffering on the socket.  Setting SO_SNDBUF
	// to 0 causes winsock to stop buffering sends and perform
	// sends directly from our buffers, thereby save one memory copy.
	//
	// However, this does prevent the socket from ever filling the
	// send pipeline. This can lead to packets being sent that are
	// not full (i.e. the overhead of the IP and TCP headers is 
	// great compared to the amount of data being carried).
	//
	// Disabling the send buffer has less serious repercussions 
	// than disabling the receive buffer.
	//

	int nZero = 0;
	int nRet = setsockopt(Sock, SOL_SOCKET, SO_SNDBUF, (char *)&nZero, sizeof(nZero));
	if (nRet == SOCKET_ERROR)
	{
		Trace()->LogErrorFormat(L"setsockopt(SNDBUF) failed: %d", WSAGetLastError());
		closesocket(Sock);
		return false;
	}

	//
	// Don't disable receive buffering. This will cause poor network
	// performance since if no receive is posted and no receive buffers,
	// the TCP stack will set the window size to zero and the peer will
	// no longer be allowed to send data.
	//

	// 
	// Do not set a linger value...especially don't set it to an abortive
	// close. If you set abortive close and there happens to be a bit of
	// data remaining to be transfered (or data that has not been 
	// acknowledged by the peer), the connection will be forcefully reset
	// and will lead to a loss of data (i.e. the peer won't get the last
	// bit of data). This is BAD. If you are worried about malicious
	// clients connecting and then not sending or receiving, the server
	// should maintain a timer on each connection. If after some point,
	// the server deems a connection is "stale" it can then set linger
	// to be abortive and close the connection.
	//

	/*
	LINGER lingerStruct;

	lingerStruct.l_onoff = 1;
	lingerStruct.l_linger = 0;
	nRet = setsockopt(sdSocket, SOL_SOCKET, SO_LINGER,
	(char *)&lingerStruct, sizeof(lingerStruct));
	if( nRet == SOCKET_ERROR ) {
	myprintf("setsockopt(SO_LINGER) failed: %d\n", WSAGetLastError());
	return(sdSocket);
	}
	*/

	return true;
}

void CWinsock::CloseSocket(SOCKET &sock)
{
	if (sock != INVALID_SOCKET)
	{
		closesocket(sock);
		sock = INVALID_SOCKET;
	}
}

//获取设备句柄
HANDLE CWinsock::Handle()
{
	return (HANDLE)m_socket;
}

//设备名称
const wchar* CWinsock::Name()
{
	return m_wsName;
}

//设置本设备名字
void CWinsock::SetName(wchar* wsName)
{
	wcscpy_s(m_wsName, array_size(m_wsName) - 1, wsName);
}
