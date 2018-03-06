/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: Reactor.h
* Author: 李辉龙
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

//反应器类
//Note:一般不使用该模式。Windows用WaitForMultipleObjects实现，有缺陷，当多个事件同时触发时它只响应第一个事件。
class CReactor :
	public IReactor,
	protected IEventHandler 
{
public:
	CReactor();
	virtual ~CReactor();

	//IReactor 接口实现
public:
	//注册事件（添加时间事件和对应的处理器）
	//一个事件可以有多个处理器，一个处理器也可以处理多个事件。
	//Example:
	//	IReactorImpl->RegisterHandler(AEvent, Handler1);
	//	IReactorImpl->RegisterHandler(BEvent, Handler1);
	//	IReactorImpl->RegisterHandler(AEvent, Handler2);
	//return:达到最大事件个数返回false
	bool RegisterHandler(HANDLE hEvent, IEventHandler *pHandler) override final;

	//删除事件的某个处理器
	//删除后，若该事件已无处理器则删除该事件；否则继续监听和处理该事件。
	void RemoveHandler(HANDLE hEvent, IEventHandler *pHandler) override final;

	//删除处理器
	//所有包含该处理器的事件都会删除该处理器，若某事件已无处理器则同时删除该事件。
	void RemoveHandler(IEventHandler *pHandler) override final;

	//删除事件并且删除该事件的处理器（不再监听该事件）
	//不会影响其他事件的处理器。
	void RemoveEvent(HANDLE hEvent) override final;

	//清空反应器
	void Clear() override final;

	//最大事件数量
	uint MaxEvents() const override final;

	//当前事件数量
	uint EventNumber() const override final;

	//释放本对象
	//调用后不得再使用该对象，因为函数内部会将本对象释放掉。
	void Release() override final;

	//等待事件触发并调度处理器处理。
	//Input:uWaitTime 最大等待时间（单位：ms）
	//等待一次，等待成功后，调度一次
	//return:超时返回false
	bool HandleEvents(uint uWaitTime) override final;

	//循环等待并调度处理器处理
	//Input:bRun循环控制，线程安全。
	void HandleEventsRun(std::atomic_bool &bRun) override final;

	//IEventHandler 接口实现
protected:
	//事件触发
	void OnEvent(HANDLE hEvent) override final;
	//当反应器中无此处理器时触发
	void OnRemovedHandler() override final;
	
	//执行函数
	//将事件操作函数和执行函数分开是因为两种行为是在不同线程中进行的。
	//而且调度线程会阻塞等待事件，在这段时间内是不能修改事件的。
protected:	
	//注册事件（添加时间事件和对应的处理器）
	bool RegisterHandlerExec(HANDLE hEvent, IEventHandler *pHandler);

	//删除事件的某个处理器
	void RemoveHandlerExec(HANDLE hEvent, IEventHandler *pHandler);

	//删除处理器
	void RemoveHandlerExec(IEventHandler *pHandler);

	//删除事件并且删除该事件的处理器（不再监听该事件）
	//return:返回删除下标(失败为-1)
	int RemoveEventAndHanleExec(HANDLE hEvent);

	//删除事件
	//param:index事件下标
	//return:返回删除下标(失败为-1)
	int RemoveEventExec(int index);

	//清空反应器
	void ClearExec();

	//内部功能
protected:
	//包含检查(事件及该事件处理器)
	bool Contained(HANDLE hEvent, IEventHandler *pHandler) const;

	//包含检查(事件)
	bool Contained(HANDLE hEvent) const;

	//包含检查(处理器)
	bool Contained(IEventHandler *pHandler) const;

	//操作容器（多线程操作）
private:
	enum Action
	{
		kRegisterHandler,		//注册事件
		kRemoveEventHandler,	//删除事件的某个处理器
		kRemoveHandler,			//删除处理器
		kRemoveEvent,			//删除事件并且删除该事件的处理器
		kClear,					//清除所有事件
	};
	struct EventItem
	{
		Action enAction;
		HANDLE hEvent;
		IEventHandler *pHandler;
	};
	//行为容器（生产者消费者模式）
	safe_queue<EventItem> m_safeActions;
	//行为事件
	HANDLE m_hActionEvent;

	//事件处理线程操作的成员变量
private:		
	struct EventContainer
	{
		int nNum; //事件数量(有符号防止0-1得很大值的情况)
		HANDLE hEvent[MAXIMUM_WAIT_OBJECTS]; //事件容器
		std::vector<IEventHandler*> vecHandler[MAXIMUM_WAIT_OBJECTS]; //事件派发容器
	};
	//事件容器，需保证线程安全 (该容器只在内部线程修改)
	EventContainer m_hEvents;

private:
	DISALLOW_COPY_AND_ASSIGN(CReactor);
};

