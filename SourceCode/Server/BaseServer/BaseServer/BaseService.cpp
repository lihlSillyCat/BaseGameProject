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
	do
	{
		if (!m_UIService.Start())
			break;

		if (!m_TraceHelper.LoadServiceObject(KernelModule::kTrace))
		{
			m_UIService.PrintOnView(L"������־ģ��ʧ��");
			break;
		}
		if (!m_TraceHelper.GetService()->Start(&m_UIService, L"BaseServer"))
		{
			m_UIService.PrintOnView(L"��־��������ʧ��");
			break;
		}
		Trace()->Log(L"��ʼ����������������");
		if (!m_CommonHelper.LoadServiceObject(KernelModule::kCommon))
		{
			Trace()->LogErrorFormat(L"���ع���ģ��ʧ��[%s]", kNAME_KERNEL[KernelModule::kCommon]);
			break;
		}
		if (!CommonService()->Start(Trace(), m_AppInfo.Environment()))
		{
			Trace()->LogError(L"������������ʧ��");
			break;
		}
		Trace()->Log(L"����������");
		m_pProactor = CommonService()->CreateProactor();
		if (m_pProactor == nullptr)
		{
			Trace()->LogError(L"����������ʧ��");
			break;
		}
		Trace()->Log(L"����ͨ��ģ��");
		if (!m_CommunicationHelper.LoadServiceObject(KernelModule::kCommunication))
		{
			Trace()->LogErrorFormat(L"����ͨ��ģ��ʧ��[%s]", kNAME_KERNEL[KernelModule::kCommunication]);
			break;
		}
		if (!CommunicationService()->Start(Trace(), m_pProactor, m_AppInfo.Environment()))
		{
			Trace()->LogError(L"ͨ�ŷ�������ʧ��");
			break;
		}
		if (nullptr == CommunicationService()->GetNetworkService())
		{
			Trace()->LogError(L"����ͨ�ŷ�������ʧ��");
			break;
		}
		m_bRunning = true;
		Trace()->Log(L"����������������ʼ����");
	} while (false);

	//����δ�����ɹ�����Դ
	if (!m_bRunning)
	{
		Clear();
	}

	return m_bRunning;
}

//����
void CBaseService::Shutdown()
{
	if (!m_bRunning)
	{
		return;
	}

	Trace()->Log(L"��ʼֹͣ������������");
	Trace()->Log(L"ֹͣͨ�ŷ��񡤡���");
	m_CommunicationHelper.Close();
	Trace()->Log(L"ֹͣ������������");
	m_CommonHelper.Close();
	Trace()->Log(L"ֹͣUI���񡤡���");
	m_UIService.Shutdown();
	Trace()->Log(L"��������ֹͣ");
	m_TraceHelper.Close();
	m_bRunning = false;
}

//������Դ
void CBaseService::Clear()
{
	m_CommonHelper.Close();
	m_CommunicationHelper.Close();
	m_TraceHelper.Close();
	m_UIService.Shutdown();
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
