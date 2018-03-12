/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: BaseService.h
* Author: �����
* Version: 1.0
* Date: 2018/3/5
* Description:���������࣬��Ӧ�ò��ṩ�ײ�֧�֡���������״̬��
*	1����������
*	2����ʼ����
*	3��ֹͣ���� ��3��4֮ǰ�м�״̬����������
*	4���˳����� �ͷ���Դ�� ע�⣺�ͷ���Դǰ�豣֤ÿ��ģ�������shutdown

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5    	   1.0		  build this module
************************************************************************/

#pragma once


#include "KernelLib\KernelExport.h"
#include "UIService.h"
#include "AppInfo.h"
#include "UserNetServer.h"

#define SERVER_NAME		L"GatewayServer"	//����������

//�������¼�
enum ServerEvent
{
	kCreateServer,	//��������
	kStartServer,	//��ʼ����
	kStopServer,	//ֹͣ���� 
	kRestartServer,	//��������
	kReleaseServer,	//�˳����� �ͷ���Դ�� ע�⣺�ͷ���Դǰ�豣֤ÿ��ģ�������ֹͣ
};

//������������Ϣ����ģʽ��������ѭ��ˢ��
class CBaseService
{
public:
	CBaseService();
	~CBaseService();

	//�������
public:
	//��ʼ
	bool Start();
	//����
	void Shutdown();
	//��ѭ��
	void Update();
	//�ɷ���״̬
	bool Servable() const { return m_bRunning; }

	//�������
public:
	//��־����
	ITrace* Trace() { return m_TraceHelper.GetService(); }

protected:
	//��������
	ICommonService* CommonService() { return m_CommonHelper.GetService(); }
	//ͨ�ŷ���
	ICommunicationService* CommunicationService() { return m_CommunicationHelper.GetService(); }
	//����ͨ�ŷ���
	INetworkService* NetworkService() { return m_CommunicationHelper.GetService()->GetNetworkService(); }

	//�������¼�
protected:
	//����UI����־����
	bool StartUIAndTrace();
	//����ģ�����
	bool LoadModuleService();
	//����ҵ��֧�ŷ���
	bool StartServer();
	//ֹͣ���� 
	void StopService();
	//��������
	bool RestartServer();	
	//�˳������ͷ���Դ��ע�⣺�ͷ���Դǰ�豣֤ÿ��ģ�������ֹͣ
	void ReleaseServer();

	//�ڲ�����
protected:

	//��Ա����
private:
	bool m_bRunning;	//����״̬
	
	//�������
private:
	//Ӧ�ó�����Ϣ
	CAppInfo m_AppInfo;
	//UI����
	CUIService m_UIService;
	//���ݷ���
	//....
	IProactor* m_pProactor;

	//����ģ��
private:
	//�������
	CKernelModuleAssistant<ICommonService> m_CommonHelper;
	//ͨ��ģ��
	CKernelModuleAssistant<ICommunicationService> m_CommunicationHelper;
	//��־����
	CKernelModuleAssistant<ITraceService> m_TraceHelper;

	//�ڲ�����
private:
	//�����������
	CUserNetServer m_UserNetServer;
};

