/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: BaseService.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/5
* Description:基础服务类，给应用层提供底层支持

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5    	   1.0		  build this module
************************************************************************/

#pragma once


#include "KernelLib\KernelExport.h"
#include "UIService.h"
#include "AppInfo.h"

//服务器采用消息驱动模式，非主动循环刷新
class CBaseService
{
public:
	CBaseService();
	~CBaseService();

	//服务控制
public:
	//开始
	bool Start();
	//结束
	void Shutdown();
	//主循环
	void Update();
	//可服务状态
	bool Servable() const { return m_bRunning; }

	//功能组件
public:
	//日志服务
	ITrace* Trace() { return m_TraceHelper.GetService(); }

protected:
	//公共服务
	ICommonService* CommonService() { return m_CommonHelper.GetService(); }
	//通信服务
	ICommunicationService* CommunicationService() { return m_CommunicationHelper.GetService(); }
	//网络通信服务
	INetworkService* NetworkService() { return m_CommunicationHelper.GetService()->GetNetworkService(); }

	//内部功能
protected:
	//清理资源
	void Clear();

	//成员变量
private:
	bool m_bRunning;	//运行状态
	
	//服务组件
private:
	//应用程序信息
	CAppInfo m_AppInfo;
	//UI服务
	CUIService m_UIService;
	//数据服务
	//....
	//公共组件
	CKernelModuleAssistant<ICommonService> m_CommonHelper;
	IProactor* m_pProactor;
	//通信模块
	CKernelModuleAssistant<ICommunicationService> m_CommunicationHelper;
	//日志服务
	CKernelModuleAssistant<ITraceService> m_TraceHelper;
};

