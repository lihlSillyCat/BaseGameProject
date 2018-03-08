/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: IModule.h
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

//服务接口
interface IService
{
	//启动服务(继承类自定义启动函数)
	//virtual bool Start(userdefined param) = 0;
	//停止服务
	//停止后不得再使用该对象，因为函数内部会将本对象释放掉。
	virtual void Shutdown() = 0;
	//服务状态
	virtual bool Serviceable() = 0;
};
