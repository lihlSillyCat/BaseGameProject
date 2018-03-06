/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: iocp.cpp
* Author: �����
* Version: 1.0
* Date: 2017/12/26
* Description:��װ��ɶ˿�

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/26		   1.0		  build this module
************************************************************************/
#include "stdafx.h"
#include "iocp.h"

//��ȡ���ʵ��߳���
uint CIoCP::GetPreferredThreadsNum()
{
	SYSTEM_INFO si;
	::GetSystemInfo(&si);

	//�����Ƽ�ѡ��:��CPU����*2���͡�CPU����*2+2�� 
	return si.dwNumberOfProcessors * 2;
}

CIoCP::CIoCP():
	m_nThreadsNum(0)
{

}

CIoCP::~CIoCP()
{
	Release();
}

//������ɶ˿�
bool CIoCP::Create(uint nNumThreads)
{
	//��ֹ�ظ�����
	if (ValidHandle(m_hIoCP))
	{
		Trace()->LogError(L"�ظ�������ɶ˿�");
		return true;
	}

	m_nThreadsNum = nNumThreads;

	m_hIoCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_nThreadsNum);

	if (INVALID_HANDLE_VALUE == m_hIoCP)
	{
		Trace()->LogErrorFormat(L"������ɶ˿�ʧ�ܣ�������[%d]", GetLastError());
		return false;
	}

	return true;
}

//�ͷ���ɶ˿�
void CIoCP::Release()
{
	if (INVALID_HANDLE_VALUE != m_hIoCP)
	{
		::CloseHandle(m_hIoCP);
		m_hIoCP = INVALID_HANDLE_VALUE;
	}
}

//�豸����
bool CIoCP::AssociateDevice(HANDLE hDevice, ULONG_PTR CompletionKey)
{
	if (InvalidHandle(hDevice))
	{
		return false;
	}

	if (InvalidHandle(m_hIoCP))
	{
		if (!Create(m_nThreadsNum))
		{
			return false;
		}
	}

	//�����豸
	HANDLE handle = ::CreateIoCompletionPort(hDevice, m_hIoCP, CompletionKey, m_nThreadsNum);
	if (NULL == handle)
	{
		Trace()->LogErrorFormat(L"��ɶ˿ڹ����豸ʧ�ܣ�������[%d]", ::GetLastError());
		return false;
	}
	
	return (handle == m_hIoCP);
}

//Ͷ�������IO��Ϣ
//����Ϣ�ᴥ�� GetStatus
bool CIoCP::PostStatus(DWORD dwNumberOfBytesTransferred, ULONG_PTR CompletionKey, LPOVERLAPPED lpOverlapped)
{
	BOOL res = ::PostQueuedCompletionStatus(m_hIoCP, dwNumberOfBytesTransferred, CompletionKey, lpOverlapped);
	if (FALSE == res)
	{
		Trace()->LogErrorFormat(L"��ɶ˿�Ͷ��IO�����Ϣʧ�ܣ�������[%d]", ::GetLastError());
		return false;
	}

	return true;
}

//�ȴ��豸I/O��ɣ��߳�����
//������� lpNumberOfBytes��IO�Ѵ�����ֽ���
//������� lpCompletionKey���û���AssociateDeviceʱ�����KEY
//������� lpOverlapped ���û���ʹ���첽ʱ�����Overlapped
bool CIoCP::GetStatus(LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED *lpOverlapped)
{
	BOOL bRes = ::GetQueuedCompletionStatus(m_hIoCP, lpNumberOfBytes, lpCompletionKey, lpOverlapped, INFINITE);

	if (FALSE == bRes)
	{
		Trace()->LogErrorFormat(L"��ɶ˿ڻ�ȡIO���״̬ʧ�ܣ�������[%d]", ::GetLastError());
		return false;
	}

	return true;
}

