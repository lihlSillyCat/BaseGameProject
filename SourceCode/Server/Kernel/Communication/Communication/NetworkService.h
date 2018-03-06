/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: NetworkService.h
* Author: �����
* Version: 1.0
* Date: 2018/2/5
* Description:����ͨ�ŷ��񣬹�������������ͨ�ŷ����ɱ�ģ���ṩ��

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
************************************************************************/
#pragma once

//�������
//�û��������ֻ�ܴ���һ�������ṩ�����������
class CNetworkService : INetworkService
{
public:
	CNetworkService(ITrace* pTrace);
	~CNetworkService();

	//�̳� INetworkService
public:
	//��ʼ�������
	bool Start() override;
	//ֹͣ�������
	//ע�⣺���ñ������󲻵���ʹ�øö���Ӧ��ָ����Ϊ�ա�
	//�����Ҫ�ٴ�ʹ������ͨ�ŷ����������´�������
	void Shutdown() override;
	//�ɷ���״̬
	bool Serviceable() override;

	//���ܺ���
public:

	//�ڲ�����
private:
	//ϵͳ�������ݣ�������Ψһ��
	WSADATA m_wsaData;
	//��־�����������Ψһ��
	ITrace* m_pTrace;
	//����״̬
	bool m_bRunning;

};

