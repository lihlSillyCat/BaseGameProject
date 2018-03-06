/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: Winsock.h
* Author: �����
* Version: 1.0
* Date: 2018/1/5
* Description: Windows Socket 2 �������������豸�Ļ���

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/1/5     	   1.0		  build this module
************************************************************************/

#pragma once

#include "IProactor.h"

//���糣��(��һ���汾ĳЩ�����޸ĳɿ����õĲ���)
enum NetConstant
{
	kAddrBufSize = (sizeof(sockaddr_in) + 16),	//AcceptEx��С��ַ����
	kAcceptIOReqMin = 32,						//Accept��С�ȴ�����
	kAcceptIOReqMax = 256,						//Accept���ȴ�����
	kIncomingConnectionMinNum = 16,				//���ӛ�����С������
	kIncomingConnectionMaxNum = 64,				//���ӛ������������
	kMaxWSABufSize = 8192,						//������պͷ��͵���󻺴棨�ֽ�����		
};

//��������豸
class CWinsock : public IIODevice
{
public:
	CWinsock();
	virtual ~CWinsock();

	//�̳к��� IIODevice
public:
	//��ȡ�豸���
	virtual HANDLE Handle() override;
	//�豸����
	virtual const wchar* Name() override;
	//�������֪ͨ
	//���� pIORequst��IO�����
	virtual void OnIOCompleted(IIORequst* pIORequst) = 0;

	//���ܺ���
public:
	//���ñ��豸����
	void SetName(wchar* wsName);
	//����״̬
	bool Working() const { return m_bRunning; }

	//�ڲ�����
protected:
	//�����첽�׽���
	bool CreateAsyncSock(SOCKET &Sock, sint nProtocol);

	//��ȫ�ر��׽���
	void CloseSocket(SOCKET &sock);

	//�ɸ��̳���ʹ�õĳ�Ա����
protected:
	//ÿ���̳��൥������SOCKET����
	SOCKET m_socket; 
	//�����ַ��Ϣ
	sockaddr_in m_sockaddr;
	//����״̬
	bool m_bRunning;
	//�豸��
	wchar m_wsName[32];

	//��ֹ�����͸�ֵ
private:
	DISALLOW_COPY_AND_ASSIGN(CWinsock);
};
