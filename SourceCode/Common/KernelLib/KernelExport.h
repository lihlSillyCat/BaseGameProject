/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: KernelModule.h
* Author: �����
* Version: 1.0
* Date: 2018/3/7
* Description:�ں�ģ�鵼�����񡣲�ͬ����ϵͳ�ò�ͬ�ĵ�����ʽ

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/7    	   1.0		  build this module
************************************************************************/

#pragma once

#include "ICommon.h"
#include "ICommunication.h"
#include "ITrace.h"

//�ں�ģ���������
template<typename ModuleService>
class CKernelModuleAssistant
{
	//Module_API��������
	typedef ModuleService*(*CreateModuleServiceProc)(const wchar* wsModuleGUID);

public:
	CKernelModuleAssistant():m_pService(nullptr) { }
	virtual ~CKernelModuleAssistant() {}

	//���ض���
	bool LoadServiceObject(KernelModule kModule)
	{
		if (NULL != m_hDLL || nullptr != m_pService || kModule < KernelModule::kCommon || kModule >= KernelModule::kModuleNum)
		{
			return false;
		}

		//����DLLģ��
		m_hDLL = ::LoadLibrary(kNAME_KERNEL[kModule]);
		if (NULL == m_hDLL)
		{
			std::wcout << L"Load [" << kNAME_KERNEL[kModule] << L"] error[" << ::GetLastError() << "]" << std::endl;
			return false;
		}

		//���뺯��
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

	//��ȡ��־����
	ModuleService* GetService() const
	{
		return m_pService;
	}

	//����״̬
	bool Serviceable()
	{
		if (m_pService != nullptr)
		{
			return m_pService->Serviceable();
		}

		return false;
	}

	//ֹͣ����
	void Shutdown()
	{
		if (nullptr != m_pService)
		{
			m_pService->Shutdown();
		}
	}

	//�ͷ���Դ
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
	HMODULE m_hDLL;	//DLLʵ��
	ModuleService* m_pService;
};
