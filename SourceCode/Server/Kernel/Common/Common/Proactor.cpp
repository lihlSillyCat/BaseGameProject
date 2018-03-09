/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: Proactor.cpp
* Author: �����
* Version: 1.0
* Date: 2017/12/27
* Description:�߲������ģʽ ������

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/27		   1.0		  build this module
************************************************************************/

#include "stdafx.h"
#include "iocp.h"
#include "Proactor.h"

////////////////////////////////////////////////////////////////////
//�����������߳�
void WorkThreadMain(CProactor* pProactor)
{
	DWORD dwNumberOfBytes = 0;
	ULONG_PTR pCompletionKey = 0;
	LPOVERLAPPED lpOverlapped = NULL;
	IIORequst* pIORequst = nullptr;

	while (pProactor->m_bRunning)
	{
		//�ȴ��豸I/O��ɣ��߳�����
		//������� lpNumberOfBytes��IO�Ѵ�����ֽ���
		//������� lpCompletionKey���û���AssociateDeviceʱ�����KEY
		//������� lpOverlapped���û���ʹ���첽ʱ�����Overlapped
		//����ֵ���ɹ����� IOCP_SUCCESS��ʧ�ܷ��ش�����
		DWORD dwCode = pProactor->m_IoCP.GetStatus(&dwNumberOfBytes, &pCompletionKey, &lpOverlapped);
		
		//����ת�������֪ͨ
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
//��������ʵ��
CProactor::CProactor() :
	m_IoCP(),
	m_bRunning(false)
{

}

CProactor::~CProactor()
{
}

//��������
bool CProactor::Start(sint nConcurrentThreads/* = -1*/)
{
	if (m_bRunning)
	{
		return true;
	}

	//������ɶ˿�
	uint num = (nConcurrentThreads < 0 ? CIoCP::GetPreferredThreadsNum() : nConcurrentThreads);
	if (!m_IoCP.Create(num))
	{
		return false;
	}

	m_bRunning = true;
	//���������߳�
	for (uint i = 0; i < num; i++)
	{
		m_WorkThreads.push_back(std::thread(WorkThreadMain, this));
	}

	return true;
}

//ֹͣ����
//ֹͣ�󲻵���ʹ�øö�����Ϊ�����ڲ��Ὣ�������ͷŵ���
void CProactor::Shutdown()
{
	//״̬���
	if (!m_bRunning)
	{
		return;
	}

	//֪ͨ�����߳����˳�
	for (size_t i = 0; i < m_WorkThreads.size(); i++)
	{
		IIORequst* pReq = new IIORequst();
		m_IoCP.PostStatus(0, CompletionKey::kExit, static_cast<LPOVERLAPPED>(pReq));
	}
	m_bRunning = false;

	//�ȴ��߳��˳�
	for (auto itr = m_WorkThreads.begin(); itr != m_WorkThreads.end(); ++itr)
	{
		itr->join();
	}

	//�ر���ɶ˿�
	m_IoCP.Release();

	//�ͷ���Դ
	m_WorkThreads.clear();

	Trace()->Log(L"��������ֹͣ");

	//ɾ������
	delete this;
}

//�ɷ���״̬
bool CProactor::Serviceable()
{
	return m_bRunning;
}

//ע���첽�豸
//���� pIODevice���첽�豸
bool CProactor::RegisterDevice(IIODevice* pIODevice)
{
	Trace()->LogFormat(L"��������ע���豸[%s]......", pIODevice->Name());

	if (!m_bRunning || nullptr == pIODevice)
	{
		return false;
	}

	//���豸�����豸ָ�뵱��KEY���룬��ɺ���ת��
	if (!m_IoCP.AssociateDevice(pIODevice->Handle(), (ULONG_PTR)pIODevice))
	{
		return false;
	}

	return true;
}
