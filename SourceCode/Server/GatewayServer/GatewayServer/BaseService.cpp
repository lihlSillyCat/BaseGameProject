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
		//����UI����־����
		if (!StartUIAndTrace())
		{
			break;
		}
		Trace()->Log(L"============================");
		Trace()->Log(L"��ʼ����������������");

		//����ģ�鲢���������
		if (!LoadModuleService())
		{
			break;
		}

		//����ҵ��֧�ŷ���
		if (!StartServer())
		{
			break;
		}

		m_bRunning = true;
		Trace()->Log(L"����������������ʼ����");

	} while (false);

	//����δ�����ɹ�����Դ
	if (!m_bRunning)
	{
		//ֹͣ����
		StopService();

		//�ͷ���Դ
		ReleaseServer();
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

	//ֹͣ����
	StopService();

	//�ͷ���Դ
	ReleaseServer();
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

//����UI����־����
bool CBaseService::StartUIAndTrace()
{
	if (!m_UIService.Start())
	{
		return false;
	}

	if (!m_TraceHelper.LoadServiceObject(KernelModule::kTrace))
	{
		m_UIService.PrintOnView(L"������־ģ��ʧ��");
		return false;
	}
	if (!m_TraceHelper.GetService()->Start(&m_UIService, SERVER_NAME))
	{
		m_UIService.PrintOnView(L"��־��������ʧ��");
		return false;
	}

	return true;
}

//����ģ�����
bool CBaseService::LoadModuleService()
{
	//����ģ��
	if (!m_CommonHelper.LoadServiceObject(KernelModule::kCommon))
	{
		Trace()->LogErrorFormat(L"���ع���ģ��ʧ��[%s]", kNAME_KERNEL[KernelModule::kCommon]);
		return false;
	}

	if (!m_CommunicationHelper.LoadServiceObject(KernelModule::kCommunication))
	{
		Trace()->LogErrorFormat(L"����ͨ��ģ��ʧ��[%s]", kNAME_KERNEL[KernelModule::kCommunication]);
		return false;
	}

	//��������
	if (!CommonService()->Start(Trace(), m_AppInfo.Environment()))
	{
		Trace()->LogError(L"������������ʧ��");
		return false;
	}
	m_pProactor = CommonService()->CreateProactor();
	if (m_pProactor == nullptr)
	{
		Trace()->LogError(L"����������ʧ��");
		return false;
	}
	if (!CommunicationService()->Start(Trace(), m_pProactor, m_AppInfo.Environment()))
	{
		Trace()->LogError(L"ͨ�ŷ�������ʧ��");
		return false;
	}
	if (nullptr == CommunicationService()->GetNetworkService())
	{
		Trace()->LogError(L"��ȡ����ͨ�ŷ���ʧ��");
		return false;
	}

	return true;
}

//����ҵ��֧�ŷ���
bool CBaseService::StartServer()
{
	//������ҷ����
	if (!m_UserNetServer.Start(NetworkService()))
	{
		Trace()->LogError(L"���������������ʧ��");
		return false;
	}

	return true;
}

//ֹͣ���� 
void CBaseService::StopService()
{
	Trace()->Log(L"��ʼֹͣ������������");

	m_UserNetServer.Shutdown();

	Trace()->Log(L"ֹͣͨ�ŷ��񡤡���");
	m_CommunicationHelper.Shutdown();

	Trace()->Log(L"ֹͣ������������");
	m_CommonHelper.Shutdown();

	Trace()->Log(L"��������ֹͣ");
	m_TraceHelper.Shutdown();

	m_bRunning = false;
}

//��������
bool CBaseService::RestartServer()
{
	Shutdown();

	return Start();
}

//�˳������ͷ���Դ��ע�⣺�ͷ���Դǰ�豣֤ÿ��ģ�������ֹͣ
void CBaseService::ReleaseServer()
{
	//ͨ��ģ��
	m_CommunicationHelper.Release();
	//�������
	m_CommonHelper.Release();
	//��־����
	m_TraceHelper.Release();
}
