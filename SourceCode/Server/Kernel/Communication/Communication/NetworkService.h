/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: NetworkService.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/5
* Description:网络通信服务，工程内所有网络通信服务都由本模块提供。

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
************************************************************************/
#pragma once

#include <vector>
#include "TCPClient.h"
#include "TCPServer.h"

//网络服务
//用户程序最多只能创建一个对象，提供所有网络服务
class CNetworkService : public INetworkService
{
public:
	CNetworkService();
	~CNetworkService();

	//继承 INetworkService
public:
	//创建网络服务器
	//参数：pConnectionHandler该网络服务器下所有客户端连接的事件处理器
	//参数：uPort范围 [1024,49151], 传入0表示系统任意指定端口
	//说明：目前仅支持TCP
	INetServer* CreateServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
		ushort uPort = 0, NetProtocol enProtocol = NetProtocol::kTCP, bool bEnableEnDecryption = true) override;

	//功能函数
public:
	//开始网络服务
	bool Start();
	//停止网络服务
	//注意：调用本函数后不得再使用该对象。应将指针置为空。
	//如果需要再次使用网络通信服务，则需重新创建对象
	void Shutdown();
	//可服务状态
	bool Serviceable();

	//功能函数
public:

	//内部功能
protected:
	//创建TCP服务端对象
	INetServer* CreateTCPServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
		ushort uPort, bool bEnableEnDecryption);

	//内部数据
private:
	//系统网络数据（工程内唯一）
	WSADATA m_wsaData;
	//运行状态
	bool m_bRunning;
	//服务器列表
	std::vector<CTCPServer*> m_vecTCPServers;
};

