/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: iocp.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/26
* Description:封装完成端口

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/26		   1.0		  build this module
************************************************************************/
#include "stdafx.h"
#include "iocp.h"

//获取合适的线程数
uint CIoCP::GetPreferredThreadsNum()
{
	SYSTEM_INFO si;
	::GetSystemInfo(&si);

	//两种推荐选择:【CPU核数*2】和【CPU核数*2+2】 
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

//创建完成端口
bool CIoCP::Create(uint nNumThreads)
{
	//禁止重复创建
	if (ValidHandle(m_hIoCP))
	{
		Trace()->LogError(L"重复创建完成端口");
		return true;
	}

	m_nThreadsNum = nNumThreads;

	m_hIoCP = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, m_nThreadsNum);

	if (INVALID_HANDLE_VALUE == m_hIoCP)
	{
		Trace()->LogErrorFormat(L"创建完成端口失败，错误码[%d]", GetLastError());
		return false;
	}

	return true;
}

//释放完成端口
void CIoCP::Release()
{
	if (INVALID_HANDLE_VALUE != m_hIoCP)
	{
		::CloseHandle(m_hIoCP);
		m_hIoCP = INVALID_HANDLE_VALUE;
	}
}

//设备关联
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

	//关联设备
	HANDLE handle = ::CreateIoCompletionPort(hDevice, m_hIoCP, CompletionKey, m_nThreadsNum);
	if (NULL == handle)
	{
		Trace()->LogErrorFormat(L"完成端口关联设备失败，错误码[%d]", ::GetLastError());
		return false;
	}
	
	return (handle == m_hIoCP);
}

//投递已完成IO消息
//该消息会触发 GetStatus
bool CIoCP::PostStatus(DWORD dwNumberOfBytesTransferred, ULONG_PTR CompletionKey, LPOVERLAPPED lpOverlapped)
{
	BOOL res = ::PostQueuedCompletionStatus(m_hIoCP, dwNumberOfBytesTransferred, CompletionKey, lpOverlapped);
	if (FALSE == res)
	{
		Trace()->LogErrorFormat(L"完成端口投递IO完成消息失败，错误码[%d]", ::GetLastError());
		return false;
	}

	return true;
}

//等待设备I/O完成，线程阻塞
//输出参数 lpNumberOfBytes：IO已传输的字节数
//输出参数 lpCompletionKey：用户在AssociateDevice时传入的KEY
//输出参数 lpOverlapped ：用户在使用异步时传入的Overlapped
bool CIoCP::GetStatus(LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED *lpOverlapped)
{
	BOOL bRes = ::GetQueuedCompletionStatus(m_hIoCP, lpNumberOfBytes, lpCompletionKey, lpOverlapped, INFINITE);

	if (FALSE == bRes)
	{
		Trace()->LogErrorFormat(L"完成端口获取IO完成状态失败，错误码[%d]", ::GetLastError());
		return false;
	}

	return true;
}

