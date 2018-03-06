#pragma once

#include <vector>

class CCommonService : public ICommonService
{
public:
	CCommonService();
	~CCommonService();

	//�̳� ICommonService
public:
	//��������
	bool Start(ITrace* pTrace, DevelopmentEnvironment DevEnvironment) override;
	//ֹͣ����
	//ֹͣ�󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
	void Shutdown() override;
	//����״̬
	bool Serviceable() override;

	//��ȡһ��������
	//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
	IProactor* CreateProactor(sint nConcurrentThreads = -1) override;

protected:
	bool m_bRunning;	//��ת״̬
	std::vector<IProactorService*> m_Proactors;
};

