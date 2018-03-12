/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: SharedRes.h
* Author: �����
* Version: 1.0
* Date: 2018/2/27
* Description:������Դ

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/27    	   1.0		  build this module
************************************************************************/

#pragma once

#include "ObjectPool.h"
#include "NetConnection.h"

//������

//��־
#define Trace() CSharedRes::Instance()->GetTrace()
//������
#define Proactor() CSharedRes::Instance()->GetProactor()
//��������
#define DevEnvironment() CSharedRes::Instance()->GetDevEnvironment()
//���Ӷ����
#define NetConnectionPool() CSharedRes::Instance()->GetNetConnectionPool()

//������Դ�ඨ��
class CSharedRes : public CSingleton<CSharedRes>
{
	//��ֹ�������󡢿�������ֵ
private:
	CSharedRes() {}
	~CSharedRes() {}
	DISALLOW_COPY_AND_ASSIGN(CSharedRes);
	friend class CSingleton<CSharedRes>;

	//��Ϣ����
public:
	//��־
	ITrace* GetTrace() const { return m_pTrace; }
	//������
	IProactor* GetProactor() const { return m_pProactor; }
	//Ӧ�ó��򿪷�����
	DevelopmentEnvironment GetDevEnvironment() const { return m_DevEnvironment; }
	//���Ӷ����(����˹���)
	CObjectPool<CNetConnection>* GetNetConnectionPool() { return &m_ConnectionPool; }

	//���ܺ���
public:
	//��־
	void SetTrace(ITrace* pTrace) { m_pTrace = pTrace; }
	//������
	void SetProactor(IProactor* pProactor) { m_pProactor = pProactor; }
	//����Ӧ�ó��򿪷�����
	void SetDevEnvironment(DevelopmentEnvironment DevEnvironment) { m_DevEnvironment = DevEnvironment; }

	//��Ա����
private:
	//��־
	ITrace* m_pTrace;
	//������
	IProactor* m_pProactor;
	//Ӧ�ó��򿪷�����
	DevelopmentEnvironment m_DevEnvironment;
	//���Ӷ����(����˹���)
	CObjectPool<CNetConnection> m_ConnectionPool;
};

