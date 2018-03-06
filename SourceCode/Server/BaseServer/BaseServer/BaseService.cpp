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

	if (!m_TraceHelper.Create(L"BaseServer", nullptr))
	{
		return false;
	}
	Trace()->Log(L"开始启动服务器···");

	Trace()->Log(L"启动UI服务");
	if (!m_UIService.Start())
	{
		return false;
	}
	if (!m_CommonHelper.Start())
	{
		return false;
	}	
	if (!m_CommonHelper.InitCommonService(m_TraceHelper.GetTrace(), m_AppInfo.Environment()))
	{
		return false;
	}
	Trace()->Log(L"启动主动器");
	m_pProactor = m_CommonHelper.CommonService()->CreateProactor();
	if (m_pProactor == nullptr)
	{
		Trace()->Log(L"启动主动器失败");
		return false;
	}

	m_bRunning = true;
	Trace()->Log(L"服务器已启动，开始工作");

	return true;
}

//结束
void CBaseService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}

	m_TraceHelper.GetTrace()->Log(L"开始停止服务器···");
	Trace()->Log(L"停止主动器···");
	m_CommonHelper.Close();
	Trace()->Log(L"停止UI服务···");
	m_UIService.Shutdown();
	m_TraceHelper.GetTrace()->Log(L"服务器已停止");
	m_TraceHelper.Close();
	m_bRunning = false;
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
