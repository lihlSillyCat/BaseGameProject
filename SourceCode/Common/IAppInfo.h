/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: AppInfo.h
* Author: �����
* Version: 1.0
* Date: 2018/2/27
* Description: Ӧ�ó�����Ϣ

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/27   	   1.0		  build this module
************************************************************************/

#pragma once

#include "SharedDefine.h"

//�������л���
enum DevelopmentEnvironment
{
	kDevelopment,	//��������
	kStaging,		//Ԥ������������ʾ����
	kProduction,	//������������ʽ��Ӫ���ϻ���
};

//�汾����
struct AppVersion
{
	ushort major;		//���汾��ǿ����
	ushort minor; 		//�ΰ汾��ǿ����
	ushort revision;	//�޶�/�׶ΰ汾(�з�����)���ȸ���
	wchar build[32];	//����汾�ţ����ڡ��ַ���˵�������ȸ���
};

//�����Ϣ
interface IComponent
{
	//���ID
	virtual uint id() = 0;

	//����汾
	virtual const AppVersion& Version() = 0;
};

//Ӧ�ó�����Ϣ�ӿ�
interface IAppInfo
{
	//�汾��Ϣ
	virtual const AppVersion& Version() = 0;

	//����������л���
	virtual DevelopmentEnvironment Environment() = 0;

	//�����Ϣ
	virtual uint GetComponents(IComponent* pComponents[], uint nSize) = 0;
};
