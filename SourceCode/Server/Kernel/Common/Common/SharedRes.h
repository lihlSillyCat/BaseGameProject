/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: SharedRes.h
* Author: �����
* Version: 1.0
* Date: 2018/3/5
* Description:������Դ

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5    	   1.0		  build this module
************************************************************************/

#pragma once
#include "singleton.h"
#include "IAppInfo.h"

//������

//��־
#define Trace() CSharedRes::Instance()->GetTrace()
//��������
#define DevEnvironment() CSharedRes::Instance()->GetDevEnvironment()

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
	ITrace * GetTrace() const { return m_pTrace; }
	//Ӧ�ó��򿪷�����
	DevelopmentEnvironment GetDevEnvironment() const { return m_DevEnvironment; }

	//���ܺ���
public:
	//��־
	void SetTrace(ITrace* pTrace) { m_pTrace = pTrace; }
	//����Ӧ�ó��򿪷�����
	void SetDevEnvironment(DevelopmentEnvironment DevEnvironment) { m_DevEnvironment = DevEnvironment; }

	//��Ա����
private:
	//��־
	ITrace * m_pTrace;
	//Ӧ�ó��򿪷�����
	DevelopmentEnvironment m_DevEnvironment;
};

