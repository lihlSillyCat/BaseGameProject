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
* Description:内核模块导出服务

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/7    	   1.0		  build this module
************************************************************************/

#pragma once

#include "ICommon.h"
#include "ICommunication.h"
#include "ITrace.h"

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
			Close();
			return false;
		}

		m_pService = CreateService(kGUID_KERNEL[kModule]);
		if (nullptr == m_pService)
		{
			std::cout << "CreateServiceObject error[" << ::GetLastError() << "]" << std::endl;
			Close();
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

private:
	HMODULE m_hDLL;	//DLL实例
	ModuleService* m_pService;
};
