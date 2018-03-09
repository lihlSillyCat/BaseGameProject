/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: Proactor.cpp
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/27
* Description:高并发设计模式 主动器

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/27		   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "iocp.h"
#include "Proactor.h"

////////////////////////////////////////////////////////////////////
//主动器并发线程
void WorkThreadMain(CProactor* pProactor)
{
	DWORD dwNumberOfBytes = 0;
	ULONG_PTR pCompletionKey = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	IIORequst* pIORequst = nullptr;

	while (pProactor->m_bRunning)
	{
		//等待设备I/O完成，线程阻塞
		//输出参数 lpNumberOfBytes：IO已传输的字节数
		//输出参数 lpCompletionKey：用户在AssociateDevice时传入的KEY
		//输出参数 lpOverlapped：用户在使用异步时传入的Overlapped
		//返回值：成功返回 IOCP_SUCCESS；失败返回错误码
		DWORD dwCode = pProactor->m_IoCP.GetStatus(&dwNumberOfBytes, &pCompletionKey, &lpOverlapped);
		
		//参数转换，完成通知
		pIORequst = (IIORequst*)lpOverlapped;
		if (IOCP_SUCCESS == dwCode)
		{
			if (CProactor::CompletionKey::kExit != pCompletionKey)
			{
				pIORequst->uNumOfBytesTransferred = dwNumberOfBytes;
				((IIODevice*)pCompletionKey)->OnIOCompleted(pIORequst);
			}
			else
			{
				delete pIORequst;
				break;
			}
		}
		else
		{
			pIORequst->uNumOfBytesTransferred = dwNumberOfBytes;
			((IIODevice*)pCompletionKey)->OnIOCompletedError(pIORequst, dwCode);
		}
	}	
}

////////////////////////////////////////////////////////////////////
//主动器类实现
CProactor::CProactor() :
	m_IoCP(),
	m_bRunning(false)
{

}

CProactor::~CProactor()
{
}

//启动服务
bool CProactor::Start(sint nConcurrentThreads/* = -1*/)
{
	if (m_bRunning)
	{
		return true;
	}

	//创建完成端口
	uint num = (nConcurrentThreads < 0 ? CIoCP::GetPreferredThreadsNum() : nConcurrentThreads);
	if (!m_IoCP.Create(num))
	{
		return false;
	}

	m_bRunning = true;
	//启动并发线程
	for (uint i = 0; i < num; i++)
	{
		m_WorkThreads.push_back(std::thread(WorkThreadMain, this));
	}

	return true;
}

//停止服务
//停止后不得再使用该对象，因为函数内部会将本对象释放掉。
void CProactor::Shutdown()
{
	//状态检测
	if (!m_bRunning)
	{
		return;
	}

	//通知工作线程组退出
	for (size_t i = 0; i < m_WorkThreads.size(); i++)
	{
		IIORequst* pReq = new IIORequst();
		m_IoCP.PostStatus(0, CompletionKey::kExit, static_cast<LPOVERLAPPED>(pReq));
	}
	m_bRunning = false;

	//等待线程退出
	for (auto itr = m_WorkThreads.begin(); itr != m_WorkThreads.end(); ++itr)
	{
		itr->join();
	}

	//关闭完成端口
	m_IoCP.Release();

	//释放资源
	m_WorkThreads.clear();

	Trace()->Log(L"主动器已停止");

	//删除自身
	delete this;
}

//可服务状态
bool CProactor::Serviceable()
{
	return m_bRunning;
}

//注册异步设备
//参数 pIODevice：异步设备
bool CProactor::RegisterDevice(IIODevice* pIODevice)
{
	Trace()->LogFormat(L"往主动器注册设备[%s]......", pIODevice->Name());

	if (!m_bRunning || nullptr == pIODevice)
	{
		return false;
	}

	//绑定设备，将设备指针当做KEY传入，完成后在转换
	if (!m_IoCP.AssociateDevice(pIODevice->Handle(), (ULONG_PTR)pIODevice))
	{
		return false;
	}

	return true;
}
