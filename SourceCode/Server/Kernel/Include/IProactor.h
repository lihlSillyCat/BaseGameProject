/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: IProactor.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/27
* Description:高并发设计模式 主动器, 接口。
注意：所有设备和IO请求皆为异步。

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/27		   1.0		  build this module
************************************************************************/

#pragma once

#include "SharedDefine.h"

//IO请求接口，所有异步请求包都必须继承该接口
//Windows平台为OVERLAPPED。其他平台根据具体情况重写
interface IIORequst : public OVERLAPPED 
{
	//异步操作完成的字节数
	ulong uNumOfBytesTransferred;	

public:
	IIORequst():
		uNumOfBytesTransferred(0)
	{
		//OVERLAPPED初始化
		Internal = 0;
		InternalHigh = 0;
		Pointer = 0;
		hEvent = 0;
	}
};

//IO设备
interface IIODevice 
{
	//获取设备句柄
	virtual HANDLE Handle() = 0;

	//设备名称
	virtual const wchar* Name() = 0;

	//操作完成通知（成功）
	//参数 pIORequst：IO请求包
	virtual void OnIOCompleted(IIORequst* pIORequst) = 0;

	//操作完成通知（失败）
	//参数 pIORequst：IO请求包
	//参数 nErrorCode ： 错误码
	virtual void OnIOCompletedError(IIORequst* pIORequst, ulong nErrorCode) = 0;
};

//主动器接口
interface IProactor
{
	//注册异步设备，注册后该设备的异步请求完成会触发其 OnCompleted 函数。
	//参数 pIODevice：异步设备
	virtual bool RegisterDevice(IIODevice* pIODevice) = 0;
};

//主动器服务类
interface IProactorService : public IProactor
{
	//启动服务
	//参数 nConcurrentThreads：并发线程数。不建议用户填写该参数。若无特殊要求则使用默认值
	virtual bool Start(sint nConcurrentThreads = -1) = 0;
	//停止服务（阻塞）
	//停止后不得再使用该对象，因为函数内部会将本对象释放掉。
	virtual void Shutdown() = 0;
	//可服务状态
	virtual bool Serviceable() = 0;
};
