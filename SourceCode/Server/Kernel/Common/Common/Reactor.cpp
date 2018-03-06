/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: Reactor.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2017/10/17
* Description:并发编程Reactor（反应器）设计模式。

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/17		   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "Reactor.h"

//反应器内部实现
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

//IReactor 接口实现
	
//注册事件（添加时间事件和对应的处理器）
bool CReactor::RegisterHandler(HANDLE hEvent, IEventHandler *pHandler)
{
	if (InvalidHandle(hEvent) || nullptr == pHandler)
	{
		return false;
	}

	//加入处理容器
	EventItem item;
	item.enAction = Action::kRegisterHandler;
	item.hEvent = hEvent;
	item.pHandler = pHandler;
	m_safeActions.push(item);

	//触发事件
	::SetEvent(m_hActionEvent);

	return true;
}

//删除事件的某个处理器
void CReactor::RemoveHandler(HANDLE hEvent, IEventHandler *pHandler)
{
	if (InvalidHandle(hEvent) || nullptr == pHandler)
	{
		return;
	}

	//加入处理容器
	EventItem item;
	item.enAction = Action::kRemoveEventHandler;
	item.hEvent = hEvent;
	item.pHandler = pHandler;
	m_safeActions.push(item);

	//触发事件
	::SetEvent(m_hActionEvent);
}

//删除处理器
void CReactor::RemoveHandler(IEventHandler *pHandler)
{
	if (nullptr == pHandler)
	{
		return;
	}

	//加入处理容器
	EventItem item;
	item.enAction = Action::kRemoveHandler;
	item.hEvent = INVALID_HANDLE_VALUE;
	item.pHandler = pHandler;
	m_safeActions.push(item);

	//触发事件
	::SetEvent(m_hActionEvent);
}

//删除事件并且删除该事件的处理器（不再监听该事件）
void CReactor::RemoveEvent(HANDLE hEvent)
{
	if (InvalidHandle(hEvent))
	{
		return;
	}

	//加入处理容器
	EventItem item;
	item.enAction = Action::kRemoveEvent;
	item.hEvent = hEvent;
	item.pHandler = nullptr;
	m_safeActions.push(item);

	//触发事件
	::SetEvent(m_hActionEvent);
}

//清空反应器
void CReactor::Clear()
{
	//加入处理容器
	EventItem item;
	item.enAction = Action::kClear;
	item.hEvent = INVALID_HANDLE_VALUE;
	item.pHandler = nullptr;
	m_safeActions.push(item);

	//触发事件
	::SetEvent(m_hActionEvent);
}

//等待事件触发并调度处理器处理。
//Input:uWaitTime 最大等待时间（单位：ms）
//等待一次，等待成功后，调度一次
//return:超时返回false
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

			//本身事件触发则继续等待应用层的事件
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

//循环等待并调度处理器处理
//Input:bRun循环控制，线程安全。
void CReactor::HandleEventsRun(std::atomic_bool &bRun)
{
	while (bRun)
	{
		HandleEvents(INFINITE);
	}
}

//最大事件数量
uint CReactor::MaxEvents() const
{
	return MAXIMUM_WAIT_OBJECTS;
}

//当前事件数量
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

//释放本对象
//调用后不得再使用该对象，因为函数内部会将本对象释放掉。
void CReactor::Release() 
{
	delete this;
}

//IEventHandler 接口实现

//事件触发
void CReactor::OnEvent(HANDLE hEvent)
{
	if (hEvent != m_hActionEvent)
	{
		return;
	}

	//数据处理
	EventItem item;
	while(m_safeActions.pop(item))
	{
		switch (item.enAction)
		{
		case Action::kRegisterHandler:		//注册事件
			RegisterHandlerExec(item.hEvent, item.pHandler);
			break;

		case Action::kRemoveEventHandler:	//删除事件的某个处理器
			RemoveHandlerExec(item.hEvent, item.pHandler);
			break;

		case Action::kRemoveHandler:		//删除处理器
			RemoveHandlerExec(item.pHandler);
			break;

		case Action::kRemoveEvent:			//删除事件并且删除该事件的处理器
			RemoveEventAndHanleExec(item.hEvent);
			break;

		case Action::kClear:				//清空反应器
			ClearExec();
			break;

		default:
			break;
		}
	}
}

//当反应器中无此处理器时触发
void CReactor::OnRemovedHandler()
{

}

//注册事件（添加时间事件和对应的处理器）
bool CReactor::RegisterHandlerExec(HANDLE hEvent, IEventHandler *pHandler)
{
	bool res = false;
	//找到当前事件，添加处理器
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (m_hEvents.hEvent[i] == hEvent)
		{
			res = true;
			m_hEvents.vecHandler[i].push_back(pHandler);
			break;
		}
	}

	//添加新事件
	if (!res && EventNumber() < MaxEvents())
	{
		m_hEvents.hEvent[m_hEvents.nNum] = hEvent;
		m_hEvents.vecHandler[m_hEvents.nNum].push_back(pHandler);
		++m_hEvents.nNum;
		res = true;
	}
	
	return res;
}

//删除事件的某个处理器
void CReactor::RemoveHandlerExec(HANDLE hEvent, IEventHandler *pHandler)
{
	//找到当前事件，删除处理器
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (m_hEvents.hEvent[i] != hEvent)
			continue;
		
		auto itr = std::find(m_hEvents.vecHandler[i].begin(), m_hEvents.vecHandler[i].end(), pHandler);
		if (itr == m_hEvents.vecHandler[i].end())
			break;

		m_hEvents.vecHandler[i].erase(itr);

		//彻底清除则通知处理器
		if (!Contained(pHandler))
		{
			pHandler->OnRemovedHandler();
		}

		//无处理器则删除该无用事件
		if (m_hEvents.vecHandler[i].empty())
		{
			RemoveEventExec(i);
		}

		break;
	}
}

//删除处理器
void CReactor::RemoveHandlerExec(IEventHandler *pHandler)
{
	bool res = false;
	//找到当前事件，删除处理器
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{	
		auto itr = std::find(m_hEvents.vecHandler[i].begin(), m_hEvents.vecHandler[i].end(), pHandler);
		if (itr == m_hEvents.vecHandler[i].end())
			continue;

		m_hEvents.vecHandler[i].erase(itr);
		res = true;

		//无处理器则删除该无用事件
		if (m_hEvents.vecHandler[i].empty())
		{
			RemoveEventExec(i);
			--i;
		}
	}

	//通知处理器
	if (res)
	{
		pHandler->OnRemovedHandler();
	}
}

//删除事件并且删除该事件的处理器（不再监听该事件）
//return:返回删除下标(失败为-1)
int CReactor::RemoveEventAndHanleExec(HANDLE hEvent)
{
	std::vector<IEventHandler*> handles;

	//找到当前事件
	int index = -1;
	for (int i = 0; i < m_hEvents.nNum; ++i)
	{
		if (m_hEvents.hEvent[i] != hEvent)
			continue;

		//移位删除
		for (int k = i; k < m_hEvents.nNum - 1; ++k)
		{
			m_hEvents.hEvent[k] = m_hEvents.hEvent[k + 1];
		}
		--m_hEvents.nNum;
		//清空处理器
		handles = m_hEvents.vecHandler[i];
		m_hEvents.vecHandler[i].clear();

		index = i;
		break;
	}

	//通知处理器
	for (auto itr = handles.begin(); itr != handles.end(); ++itr)
	{
		(*itr)->OnRemovedHandler();
	}

	return index;
}

//删除事件并且删除该事件的处理器（不再监听该事件）
//param:index事件下标
//return:返回删除下标(失败为-1)
int CReactor::RemoveEventExec(int index)
{
	if (m_hEvents.nNum >= index)
	{
		return -1;
	}

	//移位删除
	for (int k = index; k < m_hEvents.nNum - 1; ++k)
	{
		m_hEvents.hEvent[k] = m_hEvents.hEvent[k + 1];
	}
	--m_hEvents.nNum;

	return index;
}

//清空反应器
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

//包含检查(事件及该事件处理器)
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

//包含检查(事件)
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

//包含检查(处理器)
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
