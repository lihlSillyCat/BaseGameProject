/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: thread.h
* Author: �����
* Version: 1.0
* Date: 2017/12/21
* Description:�߳�ͨ��ģ��

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/21		   1.0		  build this module
************************************************************************/
#pragma once

#include <thread>
#include "SharedDefine.h"

//�̲߳����ص���ӿ�
//ͳһthread�����������̵߳���
interface IThreadHandler
{
	//�߳�����״̬
	//ʵ���෵��ֵ���п����߳���ת���˳�
	virtual bool ThreadRunning() = 0;
	//�߳̿�ʼ
	virtual void OnThreadStart() = 0;
	//�߳���ѭ����֡ˢ��
	virtual void OnThreadUpdate() = 0;
	//�߳��˳�
	virtual void OnThreadExit() = 0;
};

//�̲߳�����
interface IThreadBase : public IThreadHandler
{
public:
	IThreadBase() :
		m_pThread(nullptr)
	{
	}
	virtual ~IThreadBase()
	{
		safe_delete(m_pThread);
	}

	//�����Ե��ò���
protected:
	//�����߳�
	void StartThread(std::wstring wsName)
	{
		if (nullptr == m_pThread)
		{
			m_wsThreadName = wsName;
			m_pThread = new std::thread(ThreadMain, this);
		}
	}
	//��ȡ�߳�
	std::thread* this_thread() 
	{ 
		return m_pThread; 
	}
	//�߳���
	std::wstring ThreadName() const
	{
		return m_wsThreadName;
	}

	//��Ա����
protected:
	//�̶߳���
	std::thread* m_pThread;
	//�߳���
	std::wstring m_wsThreadName;

	//�̵߳���Main
private:
	//�̺߳���
	static void ThreadMain(IThreadHandler* pHandler)
	{
		if (nullptr == pHandler)
		{
			return;
		}
		//��ʼ
		pHandler->OnThreadStart();
		//��ת
		while (pHandler->ThreadRunning())
		{
			pHandler->OnThreadUpdate();
		}
		//�˳�
		pHandler->OnThreadExit();
	}
};
