/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: ICommonService.h
* Author: �����
* Version: 1.0
* Date: 2018/3/5
* Description:��������ģ��ӿ�

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5		   1.0		  build this module
************************************************************************/
#pragma once

#include "IAppInfo.h"

//��������ӿ�
interface IProactor;
interface ICommonService
{
	//��������
	virtual bool Start(ITrace* pTrace, DevelopmentEnvironment DevEnvironment) = 0;
	//ֹͣ����
	//ֹͣ�󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
	virtual void Shutdown() = 0;
	//����״̬
	virtual bool Serviceable() = 0;

	//��ȡһ��������
	//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
	virtual IProactor* CreateProactor(sint nConcurrentThreads = -1) = 0;
};
