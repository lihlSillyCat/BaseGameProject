/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: KernelModuleDef.h
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

//�ں�ģ�鶨��
enum KernelModule
{
	kCommon,				//�������
	kCommunication,			//ͨ�ŷ���
	kDBEngine,				//���ݿ����
	kDump,					//�쳣����
	kMemoryManager,			//�ڴ�������
	kTimeAxis,				//ʱ�������
	kTrace,					//��־����

	kModuleNum,				//ģ�����
};

//�ں�ģ���GUID
const wchar kGUID_KERNEL[KernelModule::kModuleNum][38] = {
	L"20CB213B-9946-44CF-855B-0B618BE1DDAF",
	L"CC7BAD3D-5786-47CD-BC7A-EA64B449B7F2",
	L"ABB1D3B0-FA11-4CA9-8A73-81B567722740",
	L"A8241CC6-360E-48FC-9621-3D4F3A958A04",
	L"38631F1A-943D-436E-AD5A-C97412D24A3B",
	L"89989D7C-CE96-4E7A-8A2B-901DBFCC5566",
	L"E04CA865-4622-44B9-8973-6A6ED64D1558",
};

//�ں�ģ�������
const wchar kNAME_KERNEL[KernelModule::kModuleNum][20] = {
	L"Common.dll",
	L"Communication.dll",
	L"DBEngine.dll",
	L"Dump.dll",
	L"MemoryManager.dll",
	L"TimeAxis.dll",
	L"Trace.dll",
};

/*
*	Ӧ�ó���ʹ���ں�ģ�������˵����
*	1���������񣺵���ģ������� IXXXService
*	2����ʼ���񣺵��ö��õ� Start ��������������
*	3��ֹͣ���񣺵��� Shutdown ��������
*	4���ͷ���Դ������ Release() ������ģ����������Դ����ģ���Զ��ͷš�Ӧ�ó����ù�����Դ�ͷš�
*	ע�⣺����Release()֮ǰ����Ҫ�����ں�ģ�鶼 Shutdown��
*/

//����ӿ�
interface IService
{
	//��������(�̳����Զ�����������)
	//virtual bool Start(userdefined param) = 0;
	//ֹͣ����
	virtual void Shutdown() = 0;
	//����״̬
	virtual bool Serviceable() = 0;
	//�ͷ���Դ
	//���ú󲻵���ʹ�øö�����Ϊģ���ڲ��Ὣ������Դ�ͷš�
	virtual void Release() = 0;
};
