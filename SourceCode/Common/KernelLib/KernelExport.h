/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: KernelModule.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/7
* Description:内核模块导出服务。不同操作系统用不同的导出方式

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/7    	   1.0		  build this module
************************************************************************/

#pragma once

#include "ICommon.h"
#include "ICommunication.h"
#include "ITrace.h"

//内核模块加载助手
template<typename ModuleService>
class CKernelModuleAssistant
{
	//Module_API导出函数
	typedef ModuleService*(*CreateModuleServiceProc)(const wchar* wsModuleGUID);

public:
	CKernelModuleAssistant():m_pService(nullptr) { }
	virtual ~CKernelModuleAssistant() {}

	//加载对象
	bool LoadServiceObject(KernelModule kModule)
	{
		if (NULL != m_hDLL || nullptr != m_pService || kModule < KernelModule::kCommon || kModule >= KernelModule::kModuleNum)
		{
			return false;
		}

		//加载DLL模块
		m_hDLL = ::LoadLibrary(kNAME_KERNEL[kModule]);
		if (NULL == m_hDLL)
		{
			std::wcout << L"Load [" << kNAME_KERNEL[kModule] << L"] error[" << ::GetLastError() << "]" << std::endl;
			return false;
		}

		//导入函数
		CreateModuleServiceProc CreateService = (CreateModuleServiceProc)::GetProcAddress(m_hDLL, "CreateServiceObject");
		if (nullptr == CreateService)
		{
			std::cout << "GetProcAddress [CreateServiceObject] error[" << ::GetLastError() << "]" << std::endl;
			Shutdown();
			return false;
		}

		m_pService = CreateService(kGUID_KERNEL[kModule]);
		if (nullptr == m_pService)
		{
			std::cout << "CreateServiceObject error[" << ::GetLastError() << "]" << std::endl;
			Shutdown();
			return false;
		}

		return true;
	}

	//获取日志服务
	ModuleService* GetService() const
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

	//停止服务
	void Shutdown()
	{
		if (nullptr != m_pService)
		{
			m_pService->Shutdown();
		}
	}

	//释放资源
	void Release()
	{
		if (nullptr != m_pService)
		{
			m_pService->Release();
			m_pService = nullptr;
		}

		if (NULL != m_hDLL)
		{
			::FreeLibrary(m_hDLL);
			m_hDLL = NULL;
		}
	}

private:
	HMODULE m_hDLL;	//DLL实例
	ModuleService* m_pService;
};
