/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: TCPServer.h
* Author: �����
* Version: 1.0
* Date: 2018/2/5
* Description:TCP��������

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
************************************************************************/

#pragma once

#include <list>
#include <vector>
#include "ICommunication.h"
#include "ObjectPool.h"
#include "Winsock.h"
#include "NetIORequst.h"
#include "NetConnection.h"

//TCP�����������
class CTCPServer : public CWinsock, public INetServer
{
public:
	CTCPServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler, bool bEnableEnDecryption);
	virtual ~CTCPServer();

	//�̳� CWinsock
protected:
	//�������֪ͨ
	//���� pIORequst��IO�����
	void OnIOCompleted(IIORequst* pIORequst) override;

	//�̳� INetServer
public:
	//��ַ��Ϣ(ip�Ͷ˿�)
	void GetLocalAddr(ulong& ip, ushort& port) override;
	void GetLocalAddr(wchar* wsIP, uint nSize, ushort& port) override;
	//�ӽ���״̬
	//����false��ʾ���봫�䣻����true��ʾ���ܴ���
	bool EnableEnDecryption() override { return m_bEncryptionDecryption; }

	//���ƹ���
public:
	//��ʼ����
	bool Start(ushort uPort);
	//ֹͣ����
	void Shutdown();
	
	//��Ϣ��ȡ
public:

	//�ڲ�����
protected:
	//����һ����������SOCKET
	bool CreateListenSocket(ushort uPort);
	//��ʼ��AcceptExָ��
	bool LoadfnAcceptExPtr();
	//�����ͻ������Ӳ�����Accept
	bool CreateClientsAndAccept();

	//�첽����¼�
protected:
	//���յ��µ�����
	void OnIOCompletedAccept(CNetAcceptIOReq* pIORequst);

	//��Ա����
protected:
	//AcceptEx����ָ�루���䱣�����Ч�ʣ�
	LPFN_ACCEPTEX m_fnAcceptEx;
	//ACCEPT��IO�������
	std::vector<CNetAcceptIOReq*> m_AcceptIOReqs;
	//���Ӷ����(����˹���)
	static CObjectPool<CNetConnection> m_ConnectionPool;
	//�����ӵ�����ͻ���
	std::list<CNetConnection*>	m_ConnectedClients;
	//������¼�������
	INetServerHandler* m_pServerHandler;
	//�ͻ������ӵ��¼�������
	INetConnectionHandler* m_pConnectionHandler;
	//�ӽ���
	bool m_bEncryptionDecryption;
};

