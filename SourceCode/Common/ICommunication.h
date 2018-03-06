/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: ICommunication.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/01/02
* Description:通信服务模块，包含网络通信（TCP、UDP、HTTPS等）、进程通信（管道、共享内存、邮件槽等）

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/01/02		   1.0		  build this module
************************************************************************/

#pragma once

#include "SharedDefine.h"

//网络通信协议
//Note:目前只支持TCP协议
enum NetProtocol
{
	kTCP,	//TCP协议
	kUDP,	//UDP协议
	//kHttp,	//http协议
	//kHttps, //https协议
	//kPIPE,	//管道通信
};

//网络错误码
enum NetError
{
	kDisconnected,	//网络断开
};

//连接接口声明
interface IConnection;
interface IConnectionClient;
interface IConnectionServer;

//网络调度接口
interface IConnectionHandler
{
	//收到数据
	virtual void OnRecv(IConnection* pConnection, const void* pData, uint nDataLen) = 0;
	//网络错误
	virtual void OnError(IConnection* pConnection, NetError enError) = 0;
	//断开连接
	virtual void OnDisconnected(IConnection* pConnection, sint nReason) = 0;
};

//服务端网络调度接口
interface IConnectionServerHandler : public IConnectionHandler
{
	//收到客户端连接
	virtual void OnAccept(IConnectionClient* pConnection) = 0;
};

//客户端网络调度接口
interface IConnectionClientHandler : public IConnectionHandler
{
	//连接上服务器
	virtual void OnConnected(IConnectionServer* pConnectionServer) = 0;
};

//网络连接接口
interface IConnection
{
	//发送数据
	virtual bool Send(const void* pData, uint nDataLen) = 0;
};

//服务端网络连接接口
interface IConnectionServer : public IConnection
{

};

//客户端网络连接接口
interface IConnectionClient : public IConnection
{

};

//网络通信接口
interface INetwork
{
	//开始网络服务
	virtual bool Start(uint nIOThreadsNum) = 0;

	//停止网络服务
	virtual void Shutdown() = 0;

	//释放对象
	//注意：调用本函数后不得再使用该对象。应将指针置为空。
	virtual void Release() = 0;

	//获取推荐的IO线程数
	virtual uint GetPreferredNumIOThreads() = 0;

	//创建网络服务器
	//参数：uPort范围 [1024,49151]
	virtual IConnectionServer* CreateServer(ushort uPort, NetProtocol enProtocol, IConnectionServerHandler* pHandler) = 0;

	//创建网络客户端
	//参数：wsIPAddress 服务器IP， uPort 服务器端口
	virtual IConnectionClient* CreateClient(wchar* wsIPAddress, ushort uPort, NetProtocol enProtocol, IConnectionClientHandler* pHandler) = 0;
};

//网络连接(单条连接)
interface INetConnection : public IConnection
{
	//地址信息(ip和端口)
	//virtual bool RemoteAddr(ulong& ip, ushort& port) = 0;
	//virtual bool RemoteAddr(wchar* wsIP, ushort& port) = 0;
	//virtual bool RemoteAddr(wchar* wsIP, wchar* wsPort) = 0;

};

//网络服务端
interface INetServer
{

};

//网络客户端
interface INetClient : public INetConnection
{

};

//网络连接调度接口
interface INetConnectionHandler : public IConnectionHandler
{

};

//网络服务端调度接口
interface INetServerHandler : public IConnectionHandler
{
	//收到客户端连接
	virtual void OnAccept(INetConnection* pConnection) = 0;
};

//网络通信服务接口
interface INetworkService
{
	//开始网络服务
	virtual bool Start() = 0;

	//停止网络服务
	//注意：调用本函数后不得再使用该对象。应将指针置为空。
	//如果需要再次使用网络通信服务，则需重新创建对象
	virtual void Shutdown() = 0;

	//可服务状态
	virtual bool Serviceable() = 0;

	//创建网络服务器
	//参数：pConnectionHandler该网络服务器下所有客户端连接的事件处理器
	//参数：uPort范围 [1024,49151], 传入0表示系统任意指定端口
	//说明：目前仅支持TCP
	virtual INetServer* CreateServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
		ushort uPort = 0, NetProtocol enProtocol = NetProtocol::kTCP) = 0;

	//创建网络客户端
	//virtual bool CreateClient

	//销毁已创建的对象（服务器、客户端）
	//用户不得在用户程序中私自删除对象，所有网络对象都由网络服务模块统一管理
	//virtual void
};
