/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: AppInfo.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/27
* Description: 应用程序信息

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/27   	   1.0		  build this module
************************************************************************/

#pragma once

#include "SharedDefine.h"

//开发运行环境
enum DevelopmentEnvironment
{
	kDevelopment,	//开发环境
	kStaging,		//预发布环境，演示环境
	kProduction,	//生产环境，正式运营线上环境
};

//版本定义
struct AppVersion
{
	ushort major;		//主版本（强更）
	ushort minor; 		//次版本（强更）
	ushort revision;	//修订/阶段版本(研发测试)（热更）
	wchar build[32];	//编译版本号（日期、字符串说明）（热更）
};

//组件信息
interface IComponent
{
	//组件ID
	virtual uint id() = 0;

	//组件版本
	virtual const AppVersion& Version() = 0;
};

//应用程序信息接口
interface IAppInfo
{
	//版本信息
	virtual const AppVersion& Version() = 0;

	//软件开发运行环境
	virtual DevelopmentEnvironment Environment() = 0;

	//组件信息
	virtual uint GetComponents(IComponent* pComponents[], uint nSize) = 0;
};
