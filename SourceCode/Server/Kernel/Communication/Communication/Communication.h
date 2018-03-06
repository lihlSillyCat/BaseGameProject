#pragma once

#include "ICommunication.h"

class CNetwork : public INetwork
{
public:
	CNetwork(ITrace* pTrace);
	~CNetwork();

	//INetwork�̳�
public:
	//��ʼ�������
	bool Start(uint nIOThreadsNum) override;

	//ֹͣ�������
	void Shutdown() override;

	//�ͷŶ���
	//ע�⣺���ñ������󲻵���ʹ�øö���Ӧ��ָ����Ϊ�ա�
	void Release() override;

	//��ȡ�Ƽ���IO�߳���
	uint GetPreferredNumIOThreads() override;

	//�������������
	//������uPort��Χ [1024,49151]
	IConnectionServer* CreateServer(ushort uPort, NetProtocol enProtocol, IConnectionServerHandler* pHandler) override;

	//��������ͻ���
	IConnectionClient* CreateClient(wchar* wsIPAddress, ushort uPort, NetProtocol enProtocol, IConnectionClientHandler* pHandler) override;

	//��Ա����
protected:
	ITrace*	m_pTrace;
	WSADATA	m_wsaData;	//ϵͳ��������
	bool m_bRunning;
};
