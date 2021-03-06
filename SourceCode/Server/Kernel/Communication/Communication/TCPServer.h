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
class CTCPServer : public CWinsock, public INetServer
{
public:
	CTCPServer(INetServerHandler* pServerHandler, bool bEnableEnDecryption);
	virtual ~CTCPServer();

	//继承 CWinsock
protected:
	//操作完成通知
	//参数 pIORequst：IO请求包
	void OnIOCompleted(IIORequst* pIORequst) override;
	//操作完成通知（失败）
	//参数 pIORequst：IO请求包
	//参数 nErrorCode ： 错误码
	void OnIOCompletedError(IIORequst* pIORequst, ulong nErrorCode) override;

	//继承 INetServer
public:
	//地址信息(ip和端口)
	void GetLocalAddr(ulong& ip, ushort& port) override;
	void GetLocalAddr(wchar* wsIP, uint nSize, ushort& port) override;
	//加解密状态
	//返回false表示明码传输；返回true表示加密传输
	bool EnableEnDecryption() override { return m_bEncryptionDecryption; }

	//控制功能
public:
	//开始服务
	bool Start(ushort uPort);
	//停止服务
	void Shutdown();
	//清理资源
	void Release();

	//信息获取
public:

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
	//操作完成通知（失败）
	void OnIOCompletedAcceptError(CNetAcceptIOReq* pIORequst, ulong nErrorCode);

	//成员变量
protected:
	//AcceptEx函数指针（将其保存提高效率）
	LPFN_ACCEPTEX m_fnAcceptEx;
	//ACCEPT的IO请求队列
	std::vector<CNetAcceptIOReq*> m_AcceptIOReqs;
	//服务端事件处理器
	INetServerHandler* m_pServerHandler;
	//客户端连接的事件处理器
	INetConnectionHandler* m_pConnectionHandler;
	//加解密
	bool m_bEncryptionDecryption;
};

