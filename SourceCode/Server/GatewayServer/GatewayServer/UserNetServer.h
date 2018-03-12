/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: UserNetServer.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/8
* Description:玩家网络连接服务器

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
	kMaxUserNum = 5000,		//单网关最大支撑人数
};

class CUserNetServer : public INetServerHandler
{
public:
	CUserNetServer();
	~CUserNetServer();

	//继承 INetServerHandler
protected:
	//收到客户端连接
	void OnAccept(INetConnection* pConnection) override;
	//网络错误
	void OnError(ulong errCode) override;

public:
	//启动网络服务
	bool Start(INetworkService* pNetworkService);
	//关闭服务
	void Shutdown();

private:
	//用户对象池
	CObjectPool<CUserNetClient> m_UserProxyPool;
};

