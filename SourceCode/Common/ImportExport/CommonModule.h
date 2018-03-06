/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: CommonModule.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/16
* Description:公共组件DLL，应用程序导入DLL助手

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/21  	   1.0		  build this module
************************************************************************/
#pragma once

#include "../ICommonService.h"

//模块声明
interface IProactor;
interface IReactor;

//模块加载助手
class CCommonHelper
{
	//COMMON_API导出函数
	typedef ICommonService*(*CreateCommonServiceProc)();

public:
	CCommonHelper():m_hDLL(NULL),
		m_pCommonService(nullptr)
	{
	}
	virtual ~CCommonHelper()
	{
		Close();
	}

	//服务函数
public:
	//开始
	bool Start()
	{
		if (NULL != m_hDLL)
		{
			return true;
		}

		//加载DLL模块
		m_hDLL = ::LoadLibrary(L"Common.dll");
		if (NULL == m_hDLL)
		{
			std::cout << "Load [Common.dll] error[" << ::GetLastError() << "]" << std::endl;
			return false;
		}

		return true;
	}

	//关闭服务
	//在关闭之前应用程序需要将从Common组件创建出的所有对象都Release掉！
	void Close()
	{
		if (nullptr != m_pCommonService)
		{
			m_pCommonService->Shutdown();
			m_pCommonService = nullptr;
		}

		if (NULL != m_hDLL)
		{
			::FreeLibrary(m_hDLL);
			m_hDLL = NULL;
		}
	}

	//服务状态
	bool Serviceable()
	{
		return (NULL != m_hDLL);
	}

	//公共服务模块
	ICommonService* CommonService() { return m_pCommonService; }

public:
	//初始化公共服务模块
	bool InitCommonService(ITrace* pTrace, DevelopmentEnvironment DevEnvironment)
	{
		if (!Serviceable() || nullptr != m_pCommonService)
		{
			return false;
		}

		//导入函数
		CreateCommonServiceProc CreateComm = (CreateCommonServiceProc)::GetProcAddress(m_hDLL, "CreateCommonService");
		if (nullptr == CreateComm)
		{
			std::cout << "GetProcAddress [CreateCommonService] error[" << ::GetLastError() << "]" << std::endl;
			return false;
		}

		m_pCommonService = CreateComm();
		
		if (!m_pCommonService->Start(pTrace, DevEnvironment))
		{
			return false;
		}

		return true;
	}

	//成员变量
private:
	HMODULE m_hDLL;	//DLL实例
	ICommonService* m_pCommonService;
};
