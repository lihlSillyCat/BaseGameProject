/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: ICommon.h
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
#include "IModule.h"

//公共服务接口
interface IProactor;
interface ICommonService : public IService
{
	//启动服务
	virtual bool Start(ITrace* pTrace, DevelopmentEnvironment DevEnvironment) = 0;

	//获取一个主动器
	//参数 nConcurrentThreads：并发线程数。不建议用户填写该参数。若无特殊要求则使用默认值
	virtual IProactor* CreateProactor(sint nConcurrentThreads = -1) = 0;
};
