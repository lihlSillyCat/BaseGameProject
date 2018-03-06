/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: IReactor.h
* Author: �����
* Version: 1.0
* Date: 2017/10/17
* Description: 

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/17		   1.0		  build this module
************************************************************************/
#pragma once

#include <atomic>
#include "SharedDefine.h"

//Reactor�¼��������ӿ�
//Note:�������ڷ�Ӧ����Reactor����ע��ɹ��󣬲��ܽ��Լ�ɾ�����ͷš��ÿյȡ�
//	   ֻ����OnRemovedHandler���������ִ�����������
//	   ͬ����Event	
interface IEventHandler
{
	//�¼�����
	virtual void OnEvent(HANDLE hEvent) = 0;
	//��Ӧ����ɾ����������
	//����Ӧ�����޴˴�����ʱ����
	virtual void OnRemovedHandler() = 0;
};

/*
Description:Reactor�ӿ�, ��Ӧ�����ȴ�����¼��������¼������̰߳�ȫ��
Instructions:�����ж���߳�ʹ��ͬһ����Ӧ������������ӻ���ɾ���¼����������ȣ�����ֻ�����ҽ���һ���߳�ȥ���÷�Ӧ�����¼����Ƚӿڡ�
Example:
	�߳�A,B,C��������Ӧ����ע���ɾ���¼����߳� Work ���ϵĵȴ��¼�Ȼ����д�������
	����������̣߳�һ�������߽��̡�
*/
interface IReactor
{	
	//ע���¼������ʱ���¼��Ͷ�Ӧ�Ĵ�������
	//һ���¼������ж����������һ��������Ҳ���Դ������¼���
	//Example:
	//	IReactorImpl->RegisterHandler(AEvent, Handler1);
	//	IReactorImpl->RegisterHandler(BEvent, Handler1);
	//	IReactorImpl->RegisterHandler(AEvent, Handler2);
	//return:�ﵽ����¼���������false
	virtual bool RegisterHandler(HANDLE hEvent, IEventHandler *pHandler) = 0;
	
	//ɾ���¼���ĳ��������
	//ɾ���������¼����޴�������ɾ�����¼���������������ʹ�����¼���
	virtual void RemoveHandler(HANDLE hEvent, IEventHandler *pHandler) = 0;

	//ɾ��������
	//���а����ô��������¼�����ɾ���ô���������ĳ�¼����޴�������ͬʱɾ�����¼���
	virtual void RemoveHandler(IEventHandler *pHandler) = 0;
		
	//ɾ���¼�����ɾ�����¼��Ĵ����������ټ������¼���
	//����Ӱ�������¼��Ĵ�������
	virtual void RemoveEvent(HANDLE hEvent) = 0;

	//��շ�Ӧ��
	virtual void Clear() = 0;

	//����¼�����
	virtual uint MaxEvents() const = 0;
	
	//��ǰ�¼�����
	virtual uint EventNumber() const = 0;
	
	//�ͷű�����
	//���ú󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
	virtual void Release() = 0;

	///////////////////////////////////////////////////////////////////////
	//�¼����Ƚӿ�(ֻ����Ψһһ���̵߳���)

	//�ȴ��¼����������ȴ���������
	//Input:uWaitTime ���ȴ�ʱ�䣨��λ��ms�������޵ȴ���INFINITE��
	//�ȴ�һ�Σ��ȴ��ɹ��󣬵���һ��
	//return:��ʱ��δע���¼�����false
	virtual bool HandleEvents(uint uWaitTime) = 0;

	//ѭ���ȴ������ȴ���������
	//Input:bRunѭ�����ƣ��̰߳�ȫ��
	virtual void HandleEventsRun(std::atomic_bool &bRun) = 0;	

};

//end file
///////////////////////////////////////////////////////////