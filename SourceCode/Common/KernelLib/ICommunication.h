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

#include "KernelModuleDef.h"
#include "IAppInfo.h"
#include "ITrace.h"

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

//数据发送标志
//可多标志（|）叠加使用
enum SendFlag
{
	kNormal				= 0x0001,					//正常发送
	//应用程序发送包如果不在栈上，是new的内存则可以使用此标志，减少一次内存拷贝提高效率。
	//特别注意：在发送过程中应用程序不得操作该内存，直到发送成功通知后才可以操作。
	kDisableBuffer		= (kNormal << 1),			//禁用缓存
	KUrgentData			= (kDisableBuffer << 1),	//紧急数据（不排队，直接插入发送队列最前方）
};

//断开连接的原因
enum DisconnectedCode
{
	//客户端主动断开
	//协议错误断开
	//服务器主动断开
	//其他系统原因
};

//单条连接
interface IConnection
{
	//发送数据
	//失败返回0；成功返回本连接此次发送的唯一发送标识（发送序列号）
	virtual uint Send(const void* pData, uint nDataLen, SendFlag flag = SendFlag::kNormal) = 0;
};

//连接事件处理器
template<typename ConnectionImpl>
interface IConnectionHandler
{
	//收到数据
	virtual void OnRecv(ConnectionImpl* pConnection, const void* pData, uint nDataLen) = 0;
	//网络错误
	virtual void OnError(ConnectionImpl* pConnection, ulong errCode) = 0;
	//断开连接
	virtual void OnDisconnected(ConnectionImpl* pConnection, sint nReason) = 0;
	//发送完成
	//参数：nSerialNumber 是发送函数Send的返回值
	virtual void OnSent(ConnectionImpl* pConnection, uint nSerialNumber) = 0;
};

//网络连接
interface INetConnectionHandler;
interface INetConnection : public IConnection
{
	//地址信息(ip和端口)
	virtual void GetRemoteAddr(ulong& ip, ushort& port) = 0;
	virtual void GetRemoteAddr(wchar* wsIP, uint nSize, ushort& port) = 0;
	
	//加解密状态
	//返回false表示明码传输；返回true表示加密传输
	virtual bool EnableEnDecryption() = 0;
	
	//设置连接的事件处理
	//若不设置则默认为CreateServer时传入的统一事件处理器
	virtual bool SetHandler(INetConnectionHandler* pHandler) = 0;
	virtual INetConnectionHandler* Handler() = 0;
	
	//释放对象
	virtual void Release() = 0;
};

//网络服务端
interface INetServer
{
	//地址信息(ip和端口)
	virtual void GetLocalAddr(ulong& ip, ushort& port) = 0;
	virtual void GetLocalAddr(wchar* wsIP, uint nSize, ushort& port) = 0;
	//加解密状态
	//返回false表示明码传输；返回true表示加密传输
	virtual bool EnableEnDecryption() = 0;
};

//网络客户端
interface INetClient : public INetConnection
{

};

//网络连接调度接口
interface INetConnectionHandler : public IConnectionHandler<INetConnection>
{

};

//网络服务端调度接口
interface INetServerHandler
{
	//收到客户端连接
	virtual void OnAccept(INetConnection* pConnection) = 0;
	//网络错误
	virtual void OnError(ulong errCode) = 0;
};

//网络通信服务接口
//对于本模块创建出来的对象不得在用户程序中私自 delete，所有网络对象都由网络服务模块统一管理
interface INetworkService
{
	//创建网络服务器
	//参数：uPort范围 [1024,49151], 传入0表示系统任意指定端口
	//说明：目前仅支持TCP
	virtual INetServer* CreateServer(INetServerHandler* pServerHandler, ushort uPort = 0, NetProtocol enProtocol = NetProtocol::kTCP, bool bEnableEnDecryption = true) = 0;

	//释放网络连接


	//创建网络客户端
	//virtual bool CreateClient

	//销毁已创建的对象（服务器、客户端）
	//virtual void


};

//通信服务模块
interface IProactor;
interface ICommunicationService : public IService
{
	//开始网络服务
	virtual bool Start(ITrace* pTrace, IProactor* pProactor, DevelopmentEnvironment DevEnvironment) = 0;

	//版本信息
	virtual const AppVersion& Version() = 0;

	//获取网络服务
	//失败返回 nullptr; 成功则返回已启动成功的网络服务对象
	virtual INetworkService* GetNetworkService() = 0;
};
