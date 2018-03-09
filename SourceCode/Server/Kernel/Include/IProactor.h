/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: IProactor.h
* Author: �����
* Version: 1.0
* Date: 2017/12/27
* Description:�߲������ģʽ ������, �ӿڡ�
ע�⣺�����豸��IO�����Ϊ�첽��

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/27		   1.0		  build this module
************************************************************************/

#pragma once

#include "SharedDefine.h"

//IO����ӿڣ������첽�����������̳иýӿ�
//Windowsƽ̨ΪOVERLAPPED������ƽ̨���ݾ��������д
interface IIORequst : public OVERLAPPED 
{
	//�첽������ɵ��ֽ���
	ulong uNumOfBytesTransferred;	

public:
	IIORequst():
		uNumOfBytesTransferred(0)
	{
		//OVERLAPPED��ʼ��
		Internal = 0;
		InternalHigh = 0;
		Pointer = 0;
		hEvent = 0;
	}
};

//IO�豸
interface IIODevice 
{
	//��ȡ�豸���
	virtual HANDLE Handle() = 0;

	//�豸����
	virtual const wchar* Name() = 0;

	//�������֪ͨ���ɹ���
	//���� pIORequst��IO�����
	virtual void OnIOCompleted(IIORequst* pIORequst) = 0;

	//�������֪ͨ��ʧ�ܣ�
	//���� pIORequst��IO�����
	//���� nErrorCode �� ������
	virtual void OnIOCompletedError(IIORequst* pIORequst, ulong nErrorCode) = 0;
};

//�������ӿ�
interface IProactor
{
	//ע���첽�豸��ע�����豸���첽������ɻᴥ���� OnCompleted ������
	//���� pIODevice���첽�豸
	virtual bool RegisterDevice(IIODevice* pIODevice) = 0;
};

//������������
interface IProactorService : public IProactor
{
	//��������
	//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
	virtual bool Start(sint nConcurrentThreads = -1) = 0;
	//ֹͣ����������
	//ֹͣ�󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
	virtual void Shutdown() = 0;
	//�ɷ���״̬
	virtual bool Serviceable() = 0;
};
