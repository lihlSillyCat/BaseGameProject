/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: ICommonService.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/5
* Description:公共服务模块接口

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5		   1.0		  build this module
************************************************************************/
#pragma once

#include "IAppInfo.h"

//公共服务接口
interface IProactor;
interface ICommonService
{
	//启动服务
	virtual bool Start(ITrace* pTrace, DevelopmentEnvironment DevEnvironment) = 0;
	//停止服务
	//停止后不得再使用该对象，因为函数内部会将本对象释放掉。
	virtual void Shutdown() = 0;
	//服务状态
	virtual bool Serviceable() = 0;

	//获取一个主动器
	//参数 nConcurrentThreads：并发线程数。不建议用户填写该参数。若无特殊要求则使用默认值
	virtual IProactor* CreateProactor(sint nConcurrentThreads = -1) = 0;
};
