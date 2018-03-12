#pragma once

#include <vector>
#include "Proactor.h"

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
	void Shutdown() override;
	//����״̬
	bool Serviceable() override { return m_bRunning; }
	//�ͷ���Դ
	//���ú󲻵���ʹ�øö�����Ϊģ���ڲ��Ὣ������Դ�ͷš�
	void Release() override;

	//��ȡһ��������
	//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
	IProactor* CreateProactor(sint nConcurrentThreads = -1) override;

protected:
	bool m_bRunning;	//��ת״̬
	std::vector<CProactor*> m_Proactors;
};

