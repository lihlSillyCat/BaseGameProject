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
		//启动UI和日志服务
		if (!StartUIAndTrace())
		{
			break;
		}
		Trace()->Log(L"============================");
		Trace()->Log(L"开始启动服务器···");

		//加载模块并启动其服务
		if (!LoadModuleService())
		{
			break;
		}

		//启动业务支撑服务
		if (!StartServer())
		{
			break;
		}

		m_bRunning = true;
		Trace()->Log(L"服务器已启动，开始工作");

	} while (false);

	//清理未启动成功的资源
	if (!m_bRunning)
	{
		//停止服务
		StopService();

		//释放资源
		ReleaseServer();
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

	//停止服务
	StopService();

	//释放资源
	ReleaseServer();
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

//启动UI和日志服务
bool CBaseService::StartUIAndTrace()
{
	if (!m_UIService.Start())
	{
		return false;
	}

	if (!m_TraceHelper.LoadServiceObject(KernelModule::kTrace))
	{
		m_UIService.PrintOnView(L"加载日志模块失败");
		return false;
	}
	if (!m_TraceHelper.GetService()->Start(&m_UIService, SERVER_NAME))
	{
		m_UIService.PrintOnView(L"日志服务启动失败");
		return false;
	}

	return true;
}

//加载模块服务
bool CBaseService::LoadModuleService()
{
	//加载模块
	if (!m_CommonHelper.LoadServiceObject(KernelModule::kCommon))
	{
		Trace()->LogErrorFormat(L"加载公共模块失败[%s]", kNAME_KERNEL[KernelModule::kCommon]);
		return false;
	}

	if (!m_CommunicationHelper.LoadServiceObject(KernelModule::kCommunication))
	{
		Trace()->LogErrorFormat(L"加载通信模块失败[%s]", kNAME_KERNEL[KernelModule::kCommunication]);
		return false;
	}

	//启动服务
	if (!CommonService()->Start(Trace(), m_AppInfo.Environment()))
	{
		Trace()->LogError(L"公共服务启动失败");
		return false;
	}
	m_pProactor = CommonService()->CreateProactor();
	if (m_pProactor == nullptr)
	{
		Trace()->LogError(L"启动主动器失败");
		return false;
	}
	if (!CommunicationService()->Start(Trace(), m_pProactor, m_AppInfo.Environment()))
	{
		Trace()->LogError(L"通信服务启动失败");
		return false;
	}
	if (nullptr == CommunicationService()->GetNetworkService())
	{
		Trace()->LogError(L"获取网络通信服务失败");
		return false;
	}

	return true;
}

//启动业务支撑服务
bool CBaseService::StartServer()
{
	//启动玩家服务端
	if (!m_UserNetServer.Start(NetworkService()))
	{
		Trace()->LogError(L"启动玩家网络服务端失败");
		return false;
	}

	return true;
}

//停止服务 
void CBaseService::StopService()
{
	Trace()->Log(L"开始停止服务器···");

	m_UserNetServer.Shutdown();

	Trace()->Log(L"停止通信服务···");
	m_CommunicationHelper.Shutdown();

	Trace()->Log(L"停止主动器···");
	m_CommonHelper.Shutdown();

	Trace()->Log(L"服务器已停止");
	m_TraceHelper.Shutdown();

	m_bRunning = false;
}

//重启服务
bool CBaseService::RestartServer()
{
	Shutdown();

	return Start();
}

//退出服务，释放资源。注意：释放资源前需保证每个模块服务都已停止
void CBaseService::ReleaseServer()
{
	//通信模块
	m_CommunicationHelper.Release();
	//公共组件
	m_CommonHelper.Release();
	//日志服务
	m_TraceHelper.Release();
}
