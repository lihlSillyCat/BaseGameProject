/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: CommonModule.h
* Author: �����
* Version: 1.0
* Date: 2017/12/16
* Description:�������DLL��Ӧ�ó�����DLL����

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/21  	   1.0		  build this module
************************************************************************/
#pragma once

#include "../ICommonService.h"

//ģ������
interface IProactor;
interface IReactor;

//ģ���������
class CCommonHelper
{
	//COMMON_API��������
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

	//������
public:
	//��ʼ
	bool Start()
	{
		if (NULL != m_hDLL)
		{
			return true;
		}

		//����DLLģ��
		m_hDLL = ::LoadLibrary(L"Common.dll");
		if (NULL == m_hDLL)
		{
			std::cout << "Load [Common.dll] error[" << ::GetLastError() << "]" << std::endl;
			return false;
		}

		return true;
	}

	//�رշ���
	//�ڹر�֮ǰӦ�ó�����Ҫ����Common��������������ж���Release����
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

	//����״̬
	bool Serviceable()
	{
		return (NULL != m_hDLL);
	}

	//��������ģ��
	ICommonService* CommonService() { return m_pCommonService; }

public:
	//��ʼ����������ģ��
	bool InitCommonService(ITrace* pTrace, DevelopmentEnvironment DevEnvironment)
	{
		if (!Serviceable() || nullptr != m_pCommonService)
		{
			return false;
		}

		//���뺯��
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

	//��Ա����
private:
	HMODULE m_hDLL;	//DLLʵ��
	ICommonService* m_pCommonService;
};
