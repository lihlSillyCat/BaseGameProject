/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: SharedRes.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/5
* Description:共享资源

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5    	   1.0		  build this module
************************************************************************/

#pragma once
#include "singleton.h"
#include "IAppInfo.h"

//便利宏

//日志
#define Trace() CSharedRes::Instance()->GetTrace()
//开发环境
#define DevEnvironment() CSharedRes::Instance()->GetDevEnvironment()

//共享资源类定义
class CSharedRes : public CSingleton<CSharedRes>
{
	//禁止创建对象、拷贝、赋值
private:
	CSharedRes() {}
	~CSharedRes() {}
	DISALLOW_COPY_AND_ASSIGN(CSharedRes);
	friend class CSingleton<CSharedRes>;

	//信息函数
public:
	//日志
	ITrace * GetTrace() const { return m_pTrace; }
	//应用程序开发环境
	DevelopmentEnvironment GetDevEnvironment() const { return m_DevEnvironment; }

	//功能函数
public:
	//日志
	void SetTrace(ITrace* pTrace) { m_pTrace = pTrace; }
	//设置应用程序开发环境
	void SetDevEnvironment(DevelopmentEnvironment DevEnvironment) { m_DevEnvironment = DevEnvironment; }

	//成员变量
private:
	//日志
	ITrace * m_pTrace;
	//应用程序开发环境
	DevelopmentEnvironment m_DevEnvironment;
};

