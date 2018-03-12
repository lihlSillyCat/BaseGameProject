/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: UserNetServer.h
* Author: �����
* Version: 1.0
* Date: 2018/3/8
* Description:����������ӷ�����

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/8    	   1.0		  build this module
************************************************************************/

#pragma once

#include <vector>
#include "KernelLib\ICommunication.h"
#include "UserNetClient.h"
#include "ObjectPool.h"

enum GamewayConstant
{
	kMaxUserNum = 5000,		//���������֧������
};

class CUserNetServer : public INetServerHandler
{
public:
	CUserNetServer();
	~CUserNetServer();

	//�̳� INetServerHandler
protected:
	//�յ��ͻ�������
	void OnAccept(INetConnection* pConnection) override;
	//�������
	void OnError(ulong errCode) override;

public:
	//�����������
	bool Start(INetworkService* pNetworkService);
	//�رշ���
	void Shutdown();

private:
	//�û������
	CObjectPool<CUserNetClient> m_UserProxyPool;
};

