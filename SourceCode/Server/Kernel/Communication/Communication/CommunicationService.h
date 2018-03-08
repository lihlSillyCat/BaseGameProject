/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: CommunicationService.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/7
* Description:提供通信服务（网络通信和进程通信）

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/7    	   1.0		  build this module
************************************************************************/

#pragma once
#include "NetworkService.h"
class CCommunicationService : public ICommunicationService
{
public:
	CCommunicationService();
	~CCommunicationService();

	//继承 ICommunicationService
public:
	//开始网络服务
	bool Start(ITrace* pTrace, IProactor* pProactor, DevelopmentEnvironment DevEnvironment) override;

	//停止网络服务
	//注意：调用本函数后不得再使用该对象。应将指针置为空。
	//如果需要再次使用网络通信服务，则需重新创建对象
	void Shutdown() override;

	//可服务状态
	bool Serviceable() { return m_bRunning; }

	//版本信息
	const AppVersion& Version() override { return m_Version; }

	//获取网络服务
	//失败返回 nullptr; 成功则返回已启动成功的网络服务对象
	INetworkService* GetNetworkService() override;

protected:
	//模块版本信息
	AppVersion	m_Version;
	bool m_bRunning;
	//网络服务器
	CNetworkService m_NetworkService;
};

