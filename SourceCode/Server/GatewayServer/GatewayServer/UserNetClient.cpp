/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: UserNetClient.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/9
* Description:玩家的网络连接代理

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/9    	   1.0		  build this module
************************************************************************/

#include "UserNetClient.h"



CUserNetClient::CUserNetClient() :
	m_pConnection(nullptr)
{

}

CUserNetClient::~CUserNetClient()
{
}

//收到数据
void CUserNetClient::OnRecv(INetConnection* pConnection, const void* pData, uint nDataLen)
{

}

//网络错误
void CUserNetClient::OnError(INetConnection* pConnection, ulong errCode)
{

}

//断开连接
void CUserNetClient::OnDisconnected(INetConnection* pConnection, sint nReason)
{

}

//发送完成
//参数：nSerialNumber 是发送函数Send的返回值
void CUserNetClient::OnSent(INetConnection* pConnection, uint nSerialNumber)
{

}

//初始化
bool CUserNetClient::Init(INetConnection* pConnection)
{
	m_pConnection = pConnection;
	return true;
}
