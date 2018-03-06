/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: Reactor.cpp
* Author: �����
* Version: 1.0
* Date: 2017/10/17
* Description:�������Reactor����Ӧ�������ģʽ��

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/17		   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "Reactor.h"

//��Ӧ���ڲ�ʵ��
CReactor::CReactor()
{
	m_hEvents.nNum = 0;
	ZeroMemory(m_hEvents.hEvent, sizeof(m_hEvents.hEvent));
	m_hActionEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	RegisterHandlerExec(m_hActionEvent, this);
}

CReactor::~CReactor()
{	
	m_safeActions.clear();
	safe_close(m_hActionEvent);
	ClearExec();
}

//IReactor �ӿ�ʵ��
	
//ע���¼������ʱ���¼��Ͷ�Ӧ�Ĵ�������
bool CReactor::RegisterHandler(HANDLE hEvent, IEventHandler *pHandler)
{
	if (InvalidHandle(hEvent) || nullptr == pHandler)
	{
		return false;
	}

	//���봦������
	EventItem item;
	item.enAction = Action::kRegisterHandler;
	item.hEvent = hEvent;
	item.pHandler = pHandler;
	m_safeActions.push(item);

	//�����¼�
	::SetEvent(m_hActionEvent);

	return true;
}

//ɾ���¼���ĳ��������
void CReactor::RemoveHandler(HANDLE hEvent, IEventHandler *pHandler)
{
	if (InvalidHandle(hEvent) || nullptr == pHandler)
	{
		return;
	}

	//���봦������
	EventItem item;
	item.enAction = Action::kRemoveEventHandler;
	item.hEvent = hEvent;
	item.pHandler = pHandler;
	m_safeActions.push(item);

	//�����¼�
	::SetEvent(m_hActionEvent);
}

//ɾ��������
void CReactor::RemoveHandler(IEventHandler *pHandler)
{
	if (nullptr == pHandler)
	{
		return;
	}

	//���봦������
	EventItem item;
	item.enAction = Action::kRemoveHandler;
	item.hEvent = INVALID_HANDLE_VALUE;
	item.pHandler = pHandler;
	m_safeActions.push(item);

	//�����¼�
	::SetEvent(m_hActionEvent);
}

//ɾ���¼�����ɾ�����¼��Ĵ����������ټ������¼���
void CReactor::RemoveEvent(HANDLE hEvent)
{
	if (InvalidHandle(hEvent))
	{
		return;
	}

	//���봦������
	EventItem item;
	item.enAction = Action::kRemoveEvent;
	item.hEvent = hEvent;
	item.pHandler = nullptr;
	m_safeActions.push(item);

	//�����¼�
	::SetEvent(m_hActionEvent);
}

//��շ�Ӧ��
void CReactor::Clear()
{
	//���봦������
	EventItem item;
	item.enAction = Action::kClear;
	item.hEvent = INVALID_HANDLE_VALUE;
	item.pHandler = nullptr;
	m_safeActions.push(item);

	//�����¼�
	::SetEvent(m_hActionEvent);
}

//�ȴ��¼����������ȴ���������
//Input:uWaitTime ���ȴ�ʱ�䣨��λ��ms��
//�ȴ�һ�Σ��ȴ��ɹ��󣬵���һ��
//return:��ʱ����false
bool CReactor::HandleEvents(uint uWaitTime)
{
	if (m_hEvents.nNum < 1)
	{
		return false;
	}

	bool loop = false;
	do 
	{
		uint uRes = ::WaitForMultipleObjects(m_hEvents.nNum, m_hEvents.hEvent, FALSE, uWaitTime);
		if (WAIT_OBJECT_0 <= uRes && uRes < (WAIT_OBJECT_0 + m_hEvents.nNum))
		{
			int index = uRes - WAIT_OBJECT_0;
			for (auto itr = m_hEvents.vecHandler[index].begin(); itr != m_hEvents.vecHandler[index].end(); ++itr)
			{
				(*itr)->OnEvent(m_hEvents.hEvent[index]);
			}

			//�����¼�����������ȴ�Ӧ�ò���¼�
			loop = (m_hEvents.hEvent[index] == m_hActionEvent);
		}
		else if (uRes == WAIT_TIMEOUT)
		{
			return false;
		}
		else if (uRes == WAIT_FAILED)
		{
			return false;
		}
		else
		{
			return false;
		}

	} while (loop);

	return true;
}

//ѭ���ȴ������ȴ���������
//Input:bRunѭ�����ƣ��̰߳�ȫ��
void CReactor::HandleEventsRun(std::atomic_bool &bRun)
{
	while (bRun)
	{
		HandleEvents(INFINITE);
	}
}

//����¼�����
uint CReactor::MaxEvents() const
{
	return MAXIMUM_WAIT_OBJECTS;
}

//��ǰ�¼�����
uint CReactor::EventNumber() const
{
	if (m_hEvents.nNum > -1)
	{
		return m_hEvents.nNum;
	}
	else
	{
		return 0;
	}
}

//�ͷű�����
//���ú󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
void CReactor::Release() 
{
	delete this;
}

//IEventHandler �ӿ�ʵ��

//�¼�����
void CReactor::OnEvent(HANDLE hEvent)
{
	if (hEvent != m_hActionEvent)
	{
		return;
	}

	//���ݴ���
	EventItem item;
	while(m_safeActions.pop(item))
	{
		switch (item.enAction)
		{
		case Action::kRegisterHandler:		//ע���¼�
			RegisterHandlerExec(item.hEvent, item.pHandler);
			break;

		case Action::kRemoveEventHandler:	//ɾ���¼���ĳ��������
			RemoveHandlerExec(item.hEvent, item.pHandler);
			break;

		case Action::kRemoveHandler:		//ɾ��������
			RemoveHandlerExec(item.pHandler);
			break;

		case Action::kRemoveEvent:			//ɾ���¼�����ɾ�����¼��Ĵ�����
			RemoveEventAndHanleExec(item.hEvent);
			break;

		case Action::kClear:				//��շ�Ӧ��
			ClearExec();
			break;

		default:
			break;
		}
	}
}

//����Ӧ�����޴˴�����ʱ����
void CReactor::OnRemovedHandler()
{

}

//ע���¼������ʱ���¼��Ͷ�Ӧ�Ĵ�������
bool CReactor::RegisterHandlerExec(HANDLE hEvent, IEventHandler *pHandler)
{
	bool res = false;
	//�ҵ���ǰ�¼�����Ӵ�����
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (m_hEvents.hEvent[i] == hEvent)
		{
			res = true;
			m_hEvents.vecHandler[i].push_back(pHandler);
			break;
		}
	}

	//������¼�
	if (!res && EventNumber() < MaxEvents())
	{
		m_hEvents.hEvent[m_hEvents.nNum] = hEvent;
		m_hEvents.vecHandler[m_hEvents.nNum].push_back(pHandler);
		++m_hEvents.nNum;
		res = true;
	}
	
	return res;
}

//ɾ���¼���ĳ��������
void CReactor::RemoveHandlerExec(HANDLE hEvent, IEventHandler *pHandler)
{
	//�ҵ���ǰ�¼���ɾ��������
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (m_hEvents.hEvent[i] != hEvent)
			continue;
		
		auto itr = std::find(m_hEvents.vecHandler[i].begin(), m_hEvents.vecHandler[i].end(), pHandler);
		if (itr == m_hEvents.vecHandler[i].end())
			break;

		m_hEvents.vecHandler[i].erase(itr);

		//���������֪ͨ������
		if (!Contained(pHandler))
		{
			pHandler->OnRemovedHandler();
		}

		//�޴�������ɾ���������¼�
		if (m_hEvents.vecHandler[i].empty())
		{
			RemoveEventExec(i);
		}

		break;
	}
}

//ɾ��������
void CReactor::RemoveHandlerExec(IEventHandler *pHandler)
{
	bool res = false;
	//�ҵ���ǰ�¼���ɾ��������
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{	
		auto itr = std::find(m_hEvents.vecHandler[i].begin(), m_hEvents.vecHandler[i].end(), pHandler);
		if (itr == m_hEvents.vecHandler[i].end())
			continue;

		m_hEvents.vecHandler[i].erase(itr);
		res = true;

		//�޴�������ɾ���������¼�
		if (m_hEvents.vecHandler[i].empty())
		{
			RemoveEventExec(i);
			--i;
		}
	}

	//֪ͨ������
	if (res)
	{
		pHandler->OnRemovedHandler();
	}
}

//ɾ���¼�����ɾ�����¼��Ĵ����������ټ������¼���
//return:����ɾ���±�(ʧ��Ϊ-1)
int CReactor::RemoveEventAndHanleExec(HANDLE hEvent)
{
	std::vector<IEventHandler*> handles;

	//�ҵ���ǰ�¼�
	int index = -1;
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (m_hEvents.hEvent[i] != hEvent)
			continue;

		//��λɾ��
		for (int k = i; k < m_hEvents.nNum - 1; ++k)
		{
			m_hEvents.hEvent[k] = m_hEvents.hEvent[k + 1];
		}
		--m_hEvents.nNum;
		//��մ�����
		handles = m_hEvents.vecHandler[i];
		m_hEvents.vecHandler[i].clear();

		index = i;
		break;
	}

	//֪ͨ������
	for (auto itr = handles.begin(); itr != handles.end(); ++itr)
	{
		(*itr)->OnRemovedHandler();
	}

	return index;
}

//ɾ���¼�����ɾ�����¼��Ĵ����������ټ������¼���
//param:index�¼��±�
//return:����ɾ���±�(ʧ��Ϊ-1)
int CReactor::RemoveEventExec(int index)
{
	if (m_hEvents.nNum >= index)
	{
		return -1;
	}

	//��λɾ��
	for (int k = index; k < m_hEvents.nNum - 1; ++k)
	{
		m_hEvents.hEvent[k] = m_hEvents.hEvent[k + 1];
	}
	--m_hEvents.nNum;

	return index;
}

//��շ�Ӧ��
void CReactor::ClearExec()
{
	std::vector<IEventHandler*> handles;
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		for (auto itr = m_hEvents.vecHandler[i].cbegin(); itr != m_hEvents.vecHandler[i].cend(); ++itr)
		{
			if (handles.end() != std::find(handles.begin(), handles.end(), *itr))
			{
				handles.push_back(*itr);
			}
		}
		m_hEvents.hEvent[i] = INVALID_HANDLE_VALUE;
		m_hEvents.vecHandler[i].clear();
	}
	m_hEvents.nNum = 0;

	for (auto itr = handles.begin(); itr != handles.end(); ++itr)
	{
		(*itr)->OnRemovedHandler();
	}
}

//�������(�¼������¼�������)
bool CReactor::Contained(HANDLE hEvent, IEventHandler *pHandler) const
{
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (hEvent != m_hEvents.hEvent[i])
			continue;

		for (auto itr = m_hEvents.vecHandler[i].cbegin(); itr != m_hEvents.vecHandler[i].cend(); ++itr)
		{
			if (pHandler == *itr)
			{
				return true;
			}
		}
	}

	return false;
}

//�������(�¼�)
bool CReactor::Contained(HANDLE hEvent) const
{
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (hEvent == m_hEvents.hEvent[i])
		{
			return true;
		}
	}

	return false;
}

//�������(������)
bool CReactor::Contained(IEventHandler *pHandler)  const
{
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		for (auto itr = m_hEvents.vecHandler[i].cbegin(); itr != m_hEvents.vecHandler[i].cend(); ++itr)
		{
			if (pHandler == *itr)
			{
				return true;
			}
		}
	}
	
	return false;
}
