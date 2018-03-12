/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: CommunicationService.h
* Author: �����
* Version: 1.0
* Date: 2018/3/7
* Description:�ṩͨ�ŷ�������ͨ�źͽ���ͨ�ţ�

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

	//�̳� ICommunicationService
public:
	//��ʼ�������
	bool Start(ITrace* pTrace, IProactor* pProactor, DevelopmentEnvironment DevEnvironment) override;

	//ֹͣ����
	void Shutdown() override;

	//�ɷ���״̬
	bool Serviceable() { return m_bRunning; }

	//�ͷ���Դ
	//���ú󲻵���ʹ�øö�����Ϊģ���ڲ��Ὣ������Դ�ͷš�
	void Release() override;

	//�汾��Ϣ
	const AppVersion& Version() override { return m_Version; }

	//��ȡ�������
	//ʧ�ܷ��� nullptr; �ɹ��򷵻��������ɹ�������������
	INetworkService* GetNetworkService() override;

protected:
	//ģ��汾��Ϣ
	AppVersion	m_Version;
	bool m_bRunning;
	//���������
	CNetworkService m_NetworkService;
};

