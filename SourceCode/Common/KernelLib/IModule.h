/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: IModule.h
* Author: �����
* Version: 1.0
* Date: 2018/3/8
* Description:���з���ģ��Ĺ��Խӿ�

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5		   1.0		  build this module
************************************************************************/

#pragma once

#include "..\SharedDefine.h"

//����ӿ�
interface IService
{
	//��������(�̳����Զ�����������)
	//virtual bool Start(userdefined param) = 0;
	//ֹͣ����
	//ֹͣ�󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
	virtual void Shutdown() = 0;
	//����״̬
	virtual bool Serviceable() = 0;
};
