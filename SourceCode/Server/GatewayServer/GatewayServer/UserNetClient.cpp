/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: UserNetClient.cpp
* Author: �����
* Version: 1.0
* Date: 2018/3/9
* Description:��ҵ��������Ӵ���

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

//�յ�����
void CUserNetClient::OnRecv(INetConnection* pConnection, const void* pData, uint nDataLen)
{

}

//�������
void CUserNetClient::OnError(INetConnection* pConnection, ulong errCode)
{

}

//�Ͽ�����
void CUserNetClient::OnDisconnected(INetConnection* pConnection, sint nReason)
{

}

//�������
//������nSerialNumber �Ƿ��ͺ���Send�ķ���ֵ
void CUserNetClient::OnSent(INetConnection* pConnection, uint nSerialNumber)
{

}

//��ʼ��
bool CUserNetClient::Init(INetConnection* pConnection)
{
	m_pConnection = pConnection;
	return true;
}
