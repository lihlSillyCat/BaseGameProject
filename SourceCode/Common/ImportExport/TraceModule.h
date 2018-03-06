/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: TraceService.h
* Author: �����
* Version: 1.0
* Date: 2017/12/16
* Description:��־����DLL��Ӧ�ó�����DLL����

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/16		   1.0		  build this module
************************************************************************/
#pragma once
#include "..\ITrace.h"

//ģ���������
class CTraceHelper
{
	//TRACE_API��������
	typedef bool(*CreateTraceProc)(ITraceService** ppTrace, wchar* wsName, ITraceSink* pSink);

public:
	CTraceHelper():m_hDLL(NULL), m_pService(nullptr)
	{
	}
	virtual ~CTraceHelper() 
	{
		Close();
	}

	//���ܺ���
public:
	//������־����ģ��
	bool Create(wchar* wsName, ITraceSink* pSink)
	{
		if (NULL != m_hDLL || nullptr != m_pService)
		{
			return false;
		}

		//����DLLģ��
		m_hDLL = ::LoadLibrary(L"Trace.dll");
		if (NULL == m_hDLL)
		{
			std::cout << "Load [Trace.dll] error[" << ::GetLastError() << "]" << std::endl;
			return false;
		}

		//���뺯��
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

	//�رշ���
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

	//��ȡ��־����
	ITrace* GetTrace() const 
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

	//��Ա����
private:
	HMODULE m_hDLL;	//DLLʵ��
	ITraceService* m_pService;	//��־����
};
