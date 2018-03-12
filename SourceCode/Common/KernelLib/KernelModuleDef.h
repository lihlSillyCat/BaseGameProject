/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: KernelModuleDef.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/8
* Description:所有服务模块的共性接口

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5		   1.0		  build this module
************************************************************************/

#pragma once

#include "..\SharedDefine.h"

//内核模块定义
enum KernelModule
{
	kCommon,				//公共组件
	kCommunication,			//通信服务
	kDBEngine,				//数据库服务
	kDump,					//异常捕获
	kMemoryManager,			//内存管理服务
	kTimeAxis,				//时间轴服务
	kTrace,					//日志服务

	kModuleNum,				//模块个数
};

//内核模块的GUID
const wchar kGUID_KERNEL[KernelModule::kModuleNum][38] = {
	L"20CB213B-9946-44CF-855B-0B618BE1DDAF",
	L"CC7BAD3D-5786-47CD-BC7A-EA64B449B7F2",
	L"ABB1D3B0-FA11-4CA9-8A73-81B567722740",
	L"A8241CC6-360E-48FC-9621-3D4F3A958A04",
	L"38631F1A-943D-436E-AD5A-C97412D24A3B",
	L"89989D7C-CE96-4E7A-8A2B-901DBFCC5566",
	L"E04CA865-4622-44B9-8973-6A6ED64D1558",
};

//内核模块的名称
const wchar kNAME_KERNEL[KernelModule::kModuleNum][20] = {
	L"Common.dll",
	L"Communication.dll",
	L"DBEngine.dll",
	L"Dump.dll",
	L"MemoryManager.dll",
	L"TimeAxis.dll",
	L"Trace.dll",
};

/*
*	应用程序使用内核模块服务步骤说明：
*	1，创建服务：导出模块服务类 IXXXService
*	2，开始服务：调用对用的 Start 函数，启动服务
*	3，停止服务：调用 Shutdown 结束服务
*	4，释放资源：调用 Release() 函数，模块内所有资源都由模块自动释放。应用程序不用关心资源释放。
*	注意：调用Release()之前，需要所有内核模块都 Shutdown。
*/

//服务接口
interface IService
{
	//启动服务(继承类自定义启动函数)
	//virtual bool Start(userdefined param) = 0;
	//停止服务
	virtual void Shutdown() = 0;
	//服务状态
	virtual bool Serviceable() = 0;
	//释放资源
	//调用后不得再使用该对象，因为模块内部会将所有资源释放。
	virtual void Release() = 0;
};
