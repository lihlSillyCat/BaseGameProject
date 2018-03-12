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

	//停止服务
	void Shutdown() override;

	//可服务状态
	bool Serviceable() { return m_bRunning; }

	//释放资源
	//调用后不得再使用该对象，因为模块内部会将所有资源释放。
	void Release() override;

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

