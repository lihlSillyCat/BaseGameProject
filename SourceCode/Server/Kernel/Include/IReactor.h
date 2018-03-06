/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: IReactor.h
* Author: 李辉龙
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

//Reactor事件处理器接口
//Note:处理器在反应器（Reactor）中注册成功后，不能将自己删除、释放、置空等。
//	   只有在OnRemovedHandler触发后才能执行这类操作。
//	   同理于Event	
interface IEventHandler
{
	//事件触发
	virtual void OnEvent(HANDLE hEvent) = 0;
	//反应器中删除本处理器
	//当反应器中无此处理器时触发
	virtual void OnRemovedHandler() = 0;
};

/*
Description:Reactor接口, 反应器。等待多个事件并调用事件处理。线程安全。
Instructions:可以有多个线程使用同一个反应器，向里面添加或者删除事件，处理器等，但是只能有且仅有一个线程去调用反应器的事件调度接口。
Example:
	线程A,B,C···向反应器中注册或删除事件，线程 Work 不断的等待事件然后进行触发处理。
	多个生成者线程，一个消费者进程。
*/
interface IReactor
{	
	//注册事件（添加时间事件和对应的处理器）
	//一个事件可以有多个处理器，一个处理器也可以处理多个事件。
	//Example:
	//	IReactorImpl->RegisterHandler(AEvent, Handler1);
	//	IReactorImpl->RegisterHandler(BEvent, Handler1);
	//	IReactorImpl->RegisterHandler(AEvent, Handler2);
	//return:达到最大事件个数返回false
	virtual bool RegisterHandler(HANDLE hEvent, IEventHandler *pHandler) = 0;
	
	//删除事件的某个处理器
	//删除后，若该事件已无处理器则删除该事件；否则继续监听和处理该事件。
	virtual void RemoveHandler(HANDLE hEvent, IEventHandler *pHandler) = 0;

	//删除处理器
	//所有包含该处理器的事件都会删除该处理器，若某事件已无处理器则同时删除该事件。
	virtual void RemoveHandler(IEventHandler *pHandler) = 0;
		
	//删除事件并且删除该事件的处理器（不再监听该事件）
	//不会影响其他事件的处理器。
	virtual void RemoveEvent(HANDLE hEvent) = 0;

	//清空反应器
	virtual void Clear() = 0;

	//最大事件数量
	virtual uint MaxEvents() const = 0;
	
	//当前事件数量
	virtual uint EventNumber() const = 0;
	
	//释放本对象
	//调用后不得再使用该对象，因为函数内部会将本对象释放掉。
	virtual void Release() = 0;

	///////////////////////////////////////////////////////////////////////
	//事件调度接口(只能是唯一一个线程调用)

	//等待事件触发并调度处理器处理。
	//Input:uWaitTime 最大等待时间（单位：ms）（无限等待：INFINITE）
	//等待一次，等待成功后，调度一次
	//return:超时或未注册事件返回false
	virtual bool HandleEvents(uint uWaitTime) = 0;

	//循环等待并调度处理器处理
	//Input:bRun循环控制，线程安全。
	virtual void HandleEventsRun(std::atomic_bool &bRun) = 0;	

};

//end file
///////////////////////////////////////////////////////////