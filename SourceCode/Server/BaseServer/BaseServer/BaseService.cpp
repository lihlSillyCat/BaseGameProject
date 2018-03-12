/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: BaseService.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/5
* Description:基础服务类，给应用层提供底层支持

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5    	   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "BaseService.h"


CBaseService::CBaseService():
	m_pProactor(nullptr),
	m_bRunning(false)
{
}


CBaseService::~CBaseService()
{
}

//开始
bool CBaseService::Start()
{
	if (m_bRunning)
	{
		return true;
	}
	do
	{
		if (!m_UIService.Start())
			break;

		if (!m_TraceHelper.LoadServiceObject(KernelModule::kTrace))
		{
			m_UIService.PrintOnView(L"加载日志模块失败");
			break;
		}
		if (!m_TraceHelper.GetService()->Start(&m_UIService, L"BaseServer"))
		{
			m_UIService.PrintOnView(L"日志服务启动失败");
			break;
		}
		Trace()->Log(L"开始启动服务器···");
		if (!m_CommonHelper.LoadServiceObject(KernelModule::kCommon))
		{
			Trace()->LogErrorFormat(L"加载公共模块失败[%s]", kNAME_KERNEL[KernelModule::kCommon]);
			break;
		}
		if (!CommonService()->Start(Trace(), m_AppInfo.Environment()))
		{
			Trace()->LogError(L"公共服务启动失败");
			break;
		}
		Trace()->Log(L"启动主动器");
		m_pProactor = CommonService()->CreateProactor();
		if (m_pProactor == nullptr)
		{
			Trace()->LogError(L"启动主动器失败");
			break;
		}
		Trace()->Log(L"启动通信模块");
		if (!m_CommunicationHelper.LoadServiceObject(KernelModule::kCommunication))
		{
			Trace()->LogErrorFormat(L"加载通信模块失败[%s]", kNAME_KERNEL[KernelModule::kCommunication]);
			break;
		}
		if (!CommunicationService()->Start(Trace(), m_pProactor, m_AppInfo.Environment()))
		{
			Trace()->LogError(L"通信服务启动失败");
			break;
		}
		if (nullptr == CommunicationService()->GetNetworkService())
		{
			Trace()->LogError(L"网络通信服务启动失败");
			break;
		}
		m_bRunning = true;
		Trace()->Log(L"服务器已启动，开始工作");
	} while (false);

	//清理未启动成功的资源
	if (!m_bRunning)
	{
		Clear();
	}

	return m_bRunning;
}

//结束
void CBaseService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}

	Trace()->Log(L"开始停止服务器···");
	Trace()->Log(L"停止通信服务···");
	m_CommunicationHelper.Close();
	Trace()->Log(L"停止主动器···");
	m_CommonHelper.Close();
	Trace()->Log(L"停止UI服务···");
	m_UIService.Shutdown();
	Trace()->Log(L"服务器已停止");
	m_TraceHelper.Close();
	m_bRunning = false;
}

//清理资源
void CBaseService::Clear()
{
	m_CommonHelper.Close();
	m_CommunicationHelper.Close();
	m_TraceHelper.Close();
	m_UIService.Shutdown();
}

//主循环
void CBaseService::Update()
{
	UICmdData cmd;
	if (!m_UIService.CanvasUpdate(cmd))
	{
		return;
	}

	//执行UI命令
	switch (cmd.cmd)
	{
	case UICommand::kStart:
		break;
	case UICommand::kShutdown:
		return Shutdown();
	case UICommand::kHelp:
		m_UIService.ShowHelp();
		break;
	default:
		break;
	}
}
