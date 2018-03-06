/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: Winsock.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/1/5
* Description: Windows Socket 2 服务，所有网络设备的基类

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/1/5     	   1.0		  build this module
************************************************************************/

#pragma once

#include "IProactor.h"

//网络常量(下一个版本某些常量修改成可配置的参数)
enum NetConstant
{
	kAddrBufSize = (sizeof(sockaddr_in) + 16),	//AcceptEx最小地址长度
	kAcceptIOReqMin = 32,						//Accept最小等待数量
	kAcceptIOReqMax = 256,						//Accept最大等待数量
	kIncomingConnectionMinNum = 16,				//连接泚中最小连接数
	kIncomingConnectionMaxNum = 64,				//连接泚中最大连接数
	kMaxWSABufSize = 8192,						//网络接收和发送的最大缓存（字节数）		
};

//网络服务设备
class CWinsock : public IIODevice
{
public:
	CWinsock();
	virtual ~CWinsock();

	//继承函数 IIODevice
public:
	//获取设备句柄
	virtual HANDLE Handle() override;
	//设备名称
	virtual const wchar* Name() override;
	//操作完成通知
	//参数 pIORequst：IO请求包
	virtual void OnIOCompleted(IIORequst* pIORequst) = 0;

	//功能函数
public:
	//设置本设备名字
	void SetName(wchar* wsName);
	//工作状态
	bool Working() const { return m_bRunning; }

	//内部工具
protected:
	//创建异步套接字
	bool CreateAsyncSock(SOCKET &Sock, sint nProtocol);

	//安全关闭套接字
	void CloseSocket(SOCKET &sock);

	//可给继承类使用的成员变量
protected:
	//每个继承类单独持有SOCKET对象
	SOCKET m_socket; 
	//网络地址信息
	sockaddr_in m_sockaddr;
	//运行状态
	bool m_bRunning;
	//设备名
	wchar m_wsName[32];

	//禁止拷贝和赋值
private:
	DISALLOW_COPY_AND_ASSIGN(CWinsock);
};
