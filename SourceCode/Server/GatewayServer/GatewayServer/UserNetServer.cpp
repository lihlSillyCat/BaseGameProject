/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: LocalNetServer.h
* Author: �����
* Version: 1.0
* Date: 2018/3/8
* Description:�������������

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

//�յ��ͻ�������
void CUserNetServer::OnAccept(INetConnection* pConnection)
{
	CUserNetClient *pUserClient = m_UserProxyPool.NewObject();
	pUserClient->Init(pConnection);
	pConnection->SetHandler(pUserClient);

}

//�������
void CUserNetServer::OnError(ulong errCode)
{

}

//�����������
bool CUserNetServer::Start(INetworkService* pNetworkService)
{
	INetServer* pServer = pNetworkService->CreateServer(this, 2048);
	if (nullptr == pServer)
	{
		return false;
	}

	return true;
}

//�رշ���
void CUserNetServer::Shutdown()
{

}
