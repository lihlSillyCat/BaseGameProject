/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: LocalNetServer.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/8
* Description:本地网络服务器

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/8    	   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "UserNetServer.h"



CUserNetServer::CUserNetServer() : 
	m_UserProxyPool(GamewayConstant::kMaxUserNum)
{

}

CUserNetServer::~CUserNetServer()
{
}

//收到客户端连接
void CUserNetServer::OnAccept(INetConnection* pConnection)
{
	CUserNetClient *pUserClient = m_UserProxyPool.NewObject();
	pUserClient->Init(pConnection);
	pConnection->SetHandler(pUserClient);

}

//网络错误
void CUserNetServer::OnError(ulong errCode)
{

}

//启动网络服务
bool CUserNetServer::Start(INetworkService* pNetworkService)
{
	INetServer* pServer = pNetworkService->CreateServer(this, 2048);
	if (nullptr == pServer)
	{
		return false;
	}

	return true;
}

//关闭服务
void CUserNetServer::Shutdown()
{

}
