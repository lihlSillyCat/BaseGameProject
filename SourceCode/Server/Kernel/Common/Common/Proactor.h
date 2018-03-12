/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: Proactor.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/27
* Description:高并发设计模式 主动器

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/27		   1.0		  build this module
************************************************************************/

#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include "iocp.h"

class CProactor : public IProactor
{
public:
	CProactor();
	~CProactor();

	//继承函数 IProactor
public:
	//注册异步设备
	//参数 pIODevice：异步设备
	bool RegisterDevice(IIODevice* pIODevice) override;

	//功能函数 
public:
	//启动服务
	//参数 nConcurrentThreads：并发线程数。不建议用户填写该参数。若无特殊要求则使用默认值
	bool Start(sint nConcurrentThreads = -1) ;
	//停止服务
	void Shutdown() ;
	//可服务状态
	bool Serviceable() const { return m_bRunning; }

	//内部成员
protected:
	std::atomic_bool m_bRunning;	//运转状态
	CIoCP m_IoCP;	//完成端口对象
	std::vector<std::thread> m_WorkThreads;	//并发工作线程
	//友元函数（并发线程使用）
	friend void WorkThreadMain(CProactor* pProactor);

	//内部定义
protected:
	//异步完成标识
	enum CompletionKey
	{
		kExit = 1,	//退出通知
	};
};

