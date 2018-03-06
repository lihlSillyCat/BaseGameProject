/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: iocp.h
* Author: �����
* Version: 1.0
* Date: 2017/12/26
* Description:��װ��ɶ˿�

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/26		   1.0		  build this module
lihl		2018/01/25		   1.1		  ���ƺ����ķ�װ��ʵ�֣�������־���
************************************************************************/
#pragma once

//��ɶ˿���
class CIoCP
{
	//��̬����(ֱ�ӵ���)
public:
	//��ȡ���ʵ��߳���
	static uint GetPreferredThreadsNum();

public:
	CIoCP();
	virtual ~CIoCP();

	//���ܺ���
public:
	//������ɶ˿�
	//����nNumThreads����󲢷��߳���
	bool Create(uint nNumThreads);

	//�豸����
	//������CompletionKey���û��Զ���KEY
	bool AssociateDevice(HANDLE hDevice, ULONG_PTR CompletionKey);

	//�ȴ��豸I/O��ɣ��߳�����
	//������� lpNumberOfBytes��IO�Ѵ�����ֽ���
	//������� lpCompletionKey���û���AssociateDeviceʱ�����KEY
	//������� lpOverlapped���û���ʹ���첽ʱ�����Overlapped
	bool GetStatus(LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED *lpOverlapped);

	//����Ͷ��I/O�����Ϣ���ᴥ������ GetStatus ����
	//������� dwNumberOfBytesTransferred������GetStatus���ظ�ֵ
	//������� CompletionKey������GetStatus���ظ�ֵ
	//������� lpOverlapped������GetStatus���ظ�ֵ
	bool PostStatus(DWORD dwNumberOfBytesTransferred, ULONG_PTR CompletionKey, LPOVERLAPPED lpOverlapped);

	//�ͷ���ɶ˿�
	void Release();

	//�ڲ�����
protected:

	//��Ա����
protected:
	//��ɶ˿ھ��
	HANDLE	m_hIoCP;
	//�̳߳��߳���
	uint	m_nThreadsNum;
};

