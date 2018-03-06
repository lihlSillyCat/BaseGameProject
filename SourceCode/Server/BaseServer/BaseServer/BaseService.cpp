/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: BaseService.cpp
* Author: �����
* Version: 1.0
* Date: 2018/3/5
* Description:���������࣬��Ӧ�ò��ṩ�ײ�֧��

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

//��ʼ
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
	Trace()->Log(L"��ʼ����������������");

	Trace()->Log(L"����UI����");
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
	Trace()->Log(L"����������");
	m_pProactor = m_CommonHelper.CommonService()->CreateProactor();
	if (m_pProactor == nullptr)
	{
		Trace()->Log(L"����������ʧ��");
		return false;
	}

	m_bRunning = true;
	Trace()->Log(L"����������������ʼ����");

	return true;
}

//����
void CBaseService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}

	m_TraceHelper.GetTrace()->Log(L"��ʼֹͣ������������");
	Trace()->Log(L"ֹͣ������������");
	m_CommonHelper.Close();
	Trace()->Log(L"ֹͣUI���񡤡���");
	m_UIService.Shutdown();
	m_TraceHelper.GetTrace()->Log(L"��������ֹͣ");
	m_TraceHelper.Close();
	m_bRunning = false;
}

//��ѭ��
void CBaseService::Update()
{
	UICmdData cmd;
	if (!m_UIService.CanvasUpdate(cmd))
	{
		return;
	}

	//ִ��UI����
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
