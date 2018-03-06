/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: thread.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/21
* Description:线程通用模板

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/21		   1.0		  build this module
************************************************************************/
#pragma once

#include <thread>
#include "SharedDefine.h"

//线程操作回调类接口
//统一thread命名，标明线程调用
interface IThreadHandler
{
	//线程运行状态
	//实例类返回值自行控制线程运转和退出
	virtual bool ThreadRunning() = 0;
	//线程开始
	virtual void OnThreadStart() = 0;
	//线程主循环，帧刷新
	virtual void OnThreadUpdate() = 0;
	//线程退出
	virtual void OnThreadExit() = 0;
};

//线程操作类
interface IThreadBase : public IThreadHandler
{
public:
	IThreadBase() :
		m_pThread(nullptr)
	{
	}
	virtual ~IThreadBase()
	{
		safe_delete(m_pThread);
	}

	//子类自调用操作
protected:
	//启动线程
	void StartThread(std::wstring wsName)
	{
		if (nullptr == m_pThread)
		{
			m_wsThreadName = wsName;
			m_pThread = new std::thread(ThreadMain, this);
		}
	}
	//获取线程
	std::thread* this_thread() 
	{ 
		return m_pThread; 
	}
	//线程名
	std::wstring ThreadName() const
	{
		return m_wsThreadName;
	}

	//成员变量
protected:
	//线程对象
	std::thread* m_pThread;
	//线程名
	std::wstring m_wsThreadName;

	//线程调度Main
private:
	//线程函数
	static void ThreadMain(IThreadHandler* pHandler)
	{
		if (nullptr == pHandler)
		{
			return;
		}
		//开始
		pHandler->OnThreadStart();
		//运转
		while (pHandler->ThreadRunning())
		{
			pHandler->OnThreadUpdate();
		}
		//退出
		pHandler->OnThreadExit();
	}
};
