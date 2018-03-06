/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: singleton.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/17
* Description:单例模板

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/17		   1.0		  build this module
************************************************************************/
#pragma once

#include "SharedDefine.h"

//单例模板
template<class T>
class CSingleton
{
public:
	//获取实例
	static T* Instance()
	{
		return m_pInst;
	}

	//销毁实例（调用后，不得再使用本实例）
	static void Release()
	{
		safe_delete(m_pInst);
	}

protected:
	//类实例
	static T* m_pInst;

	//禁止用户创建实例,拷贝和赋值
protected:
	CSingleton() {}
	virtual ~CSingleton() {}
	DISALLOW_COPY_AND_ASSIGN(CSingleton);
};
//编译时创建对象，线程安全
template<class T>
T* CSingleton<T>::m_pInst = new T();


/* 单例模板用例
class CSingletonImpl : public CSingleton<CSingletonImpl>
{
	//禁止创建对象、拷贝、赋值
private:
	CSingletonImpl();
	~CSingletonImpl();
	DISALLOW_COPY_AND_ASSIGN(CSingletonImpl);
	friend class CSingleton<CSingletonImpl>;

	//功能函数
public:
	//成员变量
private:

};
*/
