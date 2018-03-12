/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: UserNetClient.h
* Author: �����
* Version: 1.0
* Date: 2018/3/9
* Description:��ҵ��������Ӵ���

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/9    	   1.0		  build this module
************************************************************************/

#pragma once

#include "KernelLib\ICommunication.h"

class CUserNetClient : public INetConnectionHandler
{
public:
	CUserNetClient();
	~CUserNetClient();

	//�̳� INetConnectionHandler
protected:
	//�յ�����
	void OnRecv(INetConnection* pConnection, const void* pData, uint nDataLen) override;
	//�������
	void OnError(INetConnection* pConnection, ulong errCode) override;
	//�Ͽ�����
	void OnDisconnected(INetConnection* pConnection, sint nReason) override;
	//�������
	//������nSerialNumber �Ƿ��ͺ���Send�ķ���ֵ
	void OnSent(INetConnection* pConnection, uint nSerialNumber) override;

	//���ܺ���
public:
	//��ʼ��
	bool Init(INetConnection* pConnection);

	//�ڲ�����
protected:
	//��������
	INetConnection* m_pConnection;
};

