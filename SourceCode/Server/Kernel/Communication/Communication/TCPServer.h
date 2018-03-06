/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: TCPServer.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/5
* Description:TCP网络服务端

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
************************************************************************/

#pragma once

#include <list>
#include <vector>
#include "ICommunication.h"
#include "ObjectPool.h"
#include "Winsock.h"
#include "NetIORequst.h"
#include "NetConnection.h"

//TCP网络服务器类
class CTCPServer : public IConnectionServer, public CWinsock
{
public:
	CTCPServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler);
	virtual ~CTCPServer();

	//继承 CWinsock
protected:
	//操作完成通知
	//参数 pIORequst：IO请求包
	virtual void OnIOCompleted(IIORequst* pIORequst) override;

	//控制功能
public:
	//开始服务
	bool Start(ushort uPort);
	//停止服务
	void Shutdown();
	////运行状态
	//bool Running() const { return m_bRunning; }

	//信息获取
public:
	//网络地址
	//输出参数 pwsIP:网络IP
	//输出参数 pPort:端口
	//bool AddrInfo(const wchar** pwsIP, ushort* pPort);

	//内部功能
protected:
	//创建一个监听类型SOCKET
	bool CreateListenSocket(ushort uPort);
	//初始化AcceptEx指针
	bool LoadfnAcceptExPtr();
	//创建客户端连接并用于Accept
	bool CreateClientsAndAccept();

	//异步完成事件
protected:
	//接收到新的连接
	void OnIOCompletedAccept(CNetAcceptIOReq* pIORequst);

	//成员变量
protected:
	//AcceptEx函数指针（将其保存提高效率）
	LPFN_ACCEPTEX m_fnAcceptEx;
	//ACCEPT的IO请求队列
	std::vector<CNetAcceptIOReq*> m_AcceptIOReqs;
	//连接对象池(服务端公用)
	static CObjectPool<CNetConnection> m_ConnectionPool;
	//已连接的网络客户端
	std::list<CNetConnection*>	m_ConnectedClients;
	//服务端事件处理器
	INetServerHandler* m_pServerHandler;
	//客户端连接的事件处理器
	INetConnectionHandler* m_pConnectionHandler;
};

