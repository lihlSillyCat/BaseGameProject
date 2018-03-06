/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: NetIORequst.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/27
* Description: 网络异步操作请求

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/27     	   1.0		  build this module
lihl		2018/3/1     	   1.1		  添加Accept的异步IO请求类
************************************************************************/

#pragma once

#include "Winsock.h"

//异步操作类型
enum NetAsyncOperation
{
	kAccept,		//服务端接收客户端主动连接
	kConnect,		//客户端主动连接服务端
	kRead,			//读取数据
	kWrite,			//发送数据
};

//网络IO请求
class CNetIORequst : public IIORequst
{
public:
	CNetIORequst(NetAsyncOperation opt);
	~CNetIORequst();

public:
	//IO操作类型
	NetAsyncOperation NetOperation;
};

//TCP网络服务端用于AcceptEx的IO请求对象
class CNetAcceptIOReq : public CNetIORequst
{
public:
	CNetAcceptIOReq();
	~CNetAcceptIOReq();

public:
	//AcceptEx接收数据缓冲区(字节流协议：FirstBlock + LocalAddress + RemoteAddress)(FirstBlock=0)
	char sDataBuf[NetConstant::kAddrBufSize * 2 + 1];
	
	//给AcceptEx的SOCKET
	SOCKET sockAccept;
};

//TCP网络服务端用于WSARecv的IO请求对象
class CNetRecvIOReq : public CNetIORequst
{
public:
	CNetRecvIOReq();
	~CNetRecvIOReq();

public:
	ulong uRecvFlags;
};

