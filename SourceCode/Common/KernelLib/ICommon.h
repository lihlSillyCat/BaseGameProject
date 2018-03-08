/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: ICommon.h
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
#include "IModule.h"

//��������ӿ�
interface IProactor;
interface ICommonService : public IService
{
	//��������
	virtual bool Start(ITrace* pTrace, DevelopmentEnvironment DevEnvironment) = 0;

	//��ȡһ��������
	//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
	virtual IProactor* CreateProactor(sint nConcurrentThreads = -1) = 0;
};
