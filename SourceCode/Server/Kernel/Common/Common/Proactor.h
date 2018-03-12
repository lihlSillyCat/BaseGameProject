/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: Proactor.h
* Author: �����
* Version: 1.0
* Date: 2017/12/27
* Description:�߲������ģʽ ������

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/27		   1.0		  build this module
************************************************************************/

#pragma once

#include <atomic>
#include <thread>
#include <vector>
#include "iocp.h"

class CProactor : public IProactor
{
public:
	CProactor();
	~CProactor();

	//�̳к��� IProactor
public:
	//ע���첽�豸
	//���� pIODevice���첽�豸
	bool RegisterDevice(IIODevice* pIODevice) override;

	//���ܺ��� 
public:
	//��������
	//���� nConcurrentThreads�������߳������������û���д�ò�������������Ҫ����ʹ��Ĭ��ֵ
	bool Start(sint nConcurrentThreads = -1) ;
	//ֹͣ����
	void Shutdown() ;
	//�ɷ���״̬
	bool Serviceable() const { return m_bRunning; }

	//�ڲ���Ա
protected:
	std::atomic_bool m_bRunning;	//��ת״̬
	CIoCP m_IoCP;	//��ɶ˿ڶ���
	std::vector<std::thread> m_WorkThreads;	//���������߳�
	//��Ԫ�����������߳�ʹ�ã�
	friend void WorkThreadMain(CProactor* pProactor);

	//�ڲ�����
protected:
	//�첽��ɱ�ʶ
	enum CompletionKey
	{
		kExit = 1,	//�˳�֪ͨ
	};
};

