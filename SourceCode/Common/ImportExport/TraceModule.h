/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: TraceService.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/16
* Description:日志服务DLL，应用程序导入DLL助手

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/16		   1.0		  build this module
************************************************************************/
#pragma once
#include "..\ITrace.h"

//模块加载助手
class CTraceHelper
{
	//TRACE_API导出函数
	typedef bool(*CreateTraceProc)(ITraceService** ppTrace, wchar* wsName, ITraceSink* pSink);

public:
	CTraceHelper():m_hDLL(NULL), m_pService(nullptr)
	{
	}
	virtual ~CTraceHelper() 
	{
		Close();
	}

	//功能函数
public:
	//创建日志服务模块
	bool Create(wchar* wsName, ITraceSink* pSink)
	{
		if (NULL != m_hDLL || nullptr != m_pService)
		{
			return false;
		}

		//加载DLL模块
		m_hDLL = ::LoadLibrary(L"Trace.dll");
		if (NULL == m_hDLL)
		{
			std::cout << "Load [Trace.dll] error[" << ::GetLastError() << "]" << std::endl;
			return false;
		}

		//导入函数
		CreateTraceProc CreateTrace = (CreateTraceProc)::GetProcAddress(m_hDLL, "CreateTrace");
		if (nullptr == CreateTrace)
		{
			std::cout << "GetProcAddress [CreateTrace] error[" << ::GetLastError() << "]" << std::endl;
			Close();
			return false;
		}

		if (!CreateTrace(&m_pService, wsName, pSink))
		{
			std::cout << "CreateTrace error[" << ::GetLastError() << "]" << std::endl;
			Close();
			return false;
		}

		if (!m_pService->Start())
		{
			std::cout << "TraceService start failed." << std::endl;
			Close();
			return false;
		}

		return true;
	}

	//关闭服务
	void Close()
	{
		if (nullptr != m_pService)
		{
			m_pService->Shutdown();
			m_pService = nullptr;
		}
		if (NULL != m_hDLL)
		{
			::FreeLibrary(m_hDLL);
			m_hDLL = NULL;
		}
	}

	//获取日志服务
	ITrace* GetTrace() const 
	{
		return m_pService;
	}

	//服务状态
	bool Serviceable()
	{
		if (m_pService != nullptr)
		{
			return m_pService->Serviceable();
		}

		return false;
	}

	//成员变量
private:
	HMODULE m_hDLL;	//DLL实例
	ITraceService* m_pService;	//日志服务
};
