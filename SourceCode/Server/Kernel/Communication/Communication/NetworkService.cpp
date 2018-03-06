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


CNetworkService::CNetworkService(ITrace* pTrace) :
	m_pTrace(pTrace),
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
		m_pTrace->LogErrorFormat(L"WSAStartup failed: %d\n", res);
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

	//清理系统网络
	WSACleanup();

	//自释放
	delete this;
}

//可服务状态
bool CNetworkService::Serviceable()
{
	return m_bRunning;
}
