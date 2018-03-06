/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: Reactor.h
* Author: �����
* Version: 1.0
* Date: 2017/10/17
* Description:

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/17		   1.0		  build this module
************************************************************************/
#pragma once

#include <unordered_map>
#include <vector>
#include <atomic>
#include "IReactor.h"
#include "ITrace.h"
#include "SafeContainer.h"

//��Ӧ����
//Note:һ�㲻ʹ�ø�ģʽ��Windows��WaitForMultipleObjectsʵ�֣���ȱ�ݣ�������¼�ͬʱ����ʱ��ֻ��Ӧ��һ���¼���
class CReactor :
	public IReactor,
	protected IEventHandler 
{
public:
	CReactor();
	virtual ~CReactor();

	//IReactor �ӿ�ʵ��
public:
	//ע���¼������ʱ���¼��Ͷ�Ӧ�Ĵ�������
	//һ���¼������ж����������һ��������Ҳ���Դ������¼���
	//Example:
	//	IReactorImpl->RegisterHandler(AEvent, Handler1);
	//	IReactorImpl->RegisterHandler(BEvent, Handler1);
	//	IReactorImpl->RegisterHandler(AEvent, Handler2);
	//return:�ﵽ����¼���������false
	bool RegisterHandler(HANDLE hEvent, IEventHandler *pHandler) override final;

	//ɾ���¼���ĳ��������
	//ɾ���������¼����޴�������ɾ�����¼���������������ʹ�����¼���
	void RemoveHandler(HANDLE hEvent, IEventHandler *pHandler) override final;

	//ɾ��������
	//���а����ô��������¼�����ɾ���ô���������ĳ�¼����޴�������ͬʱɾ�����¼���
	void RemoveHandler(IEventHandler *pHandler) override final;

	//ɾ���¼�����ɾ�����¼��Ĵ����������ټ������¼���
	//����Ӱ�������¼��Ĵ�������
	void RemoveEvent(HANDLE hEvent) override final;

	//��շ�Ӧ��
	void Clear() override final;

	//����¼�����
	uint MaxEvents() const override final;

	//��ǰ�¼�����
	uint EventNumber() const override final;

	//�ͷű�����
	//���ú󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
	void Release() override final;

	//�ȴ��¼����������ȴ���������
	//Input:uWaitTime ���ȴ�ʱ�䣨��λ��ms��
	//�ȴ�һ�Σ��ȴ��ɹ��󣬵���һ��
	//return:��ʱ����false
	bool HandleEvents(uint uWaitTime) override final;

	//ѭ���ȴ������ȴ���������
	//Input:bRunѭ�����ƣ��̰߳�ȫ��
	void HandleEventsRun(std::atomic_bool &bRun) override final;

	//IEventHandler �ӿ�ʵ��
protected:
	//�¼�����
	void OnEvent(HANDLE hEvent) override final;
	//����Ӧ�����޴˴�����ʱ����
	void OnRemovedHandler() override final;
	
	//ִ�к���
	//���¼�����������ִ�к����ֿ�����Ϊ������Ϊ���ڲ�ͬ�߳��н��еġ�
	//���ҵ����̻߳������ȴ��¼��������ʱ�����ǲ����޸��¼��ġ�
protected:	
	//ע���¼������ʱ���¼��Ͷ�Ӧ�Ĵ�������
	bool RegisterHandlerExec(HANDLE hEvent, IEventHandler *pHandler);

	//ɾ���¼���ĳ��������
	void RemoveHandlerExec(HANDLE hEvent, IEventHandler *pHandler);

	//ɾ��������
	void RemoveHandlerExec(IEventHandler *pHandler);

	//ɾ���¼�����ɾ�����¼��Ĵ����������ټ������¼���
	//return:����ɾ���±�(ʧ��Ϊ-1)
	int RemoveEventAndHanleExec(HANDLE hEvent);

	//ɾ���¼�
	//param:index�¼��±�
	//return:����ɾ���±�(ʧ��Ϊ-1)
	int RemoveEventExec(int index);

	//��շ�Ӧ��
	void ClearExec();

	//�ڲ�����
protected:
	//�������(�¼������¼�������)
	bool Contained(HANDLE hEvent, IEventHandler *pHandler) const;

	//�������(�¼�)
	bool Contained(HANDLE hEvent) const;

	//�������(������)
	bool Contained(IEventHandler *pHandler) const;

	//�������������̲߳�����
private:
	enum Action
	{
		kRegisterHandler,		//ע���¼�
		kRemoveEventHandler,	//ɾ���¼���ĳ��������
		kRemoveHandler,			//ɾ��������
		kRemoveEvent,			//ɾ���¼�����ɾ�����¼��Ĵ�����
		kClear,					//��������¼�
	};
	struct EventItem
	{
		Action enAction;
		HANDLE hEvent;
		IEventHandler *pHandler;
	};
	//��Ϊ������������������ģʽ��
	safe_queue<EventItem> m_safeActions;
	//��Ϊ�¼�
	HANDLE m_hActionEvent;

	//�¼������̲߳����ĳ�Ա����
private:		
	struct EventContainer
	{
		int nNum; //�¼�����(�з��ŷ�ֹ0-1�úܴ�ֵ�����)
		HANDLE hEvent[MAXIMUM_WAIT_OBJECTS]; //�¼�����
		std::vector<IEventHandler*> vecHandler[MAXIMUM_WAIT_OBJECTS]; //�¼��ɷ�����
	};
	//�¼��������豣֤�̰߳�ȫ (������ֻ���ڲ��߳��޸�)
	EventContainer m_hEvents;

private:
	DISALLOW_COPY_AND_ASSIGN(CReactor);
};

