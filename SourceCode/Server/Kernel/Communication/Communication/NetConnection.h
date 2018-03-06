#pragma once

#include "Winsock.h"
#include "NetIORequst.h"

//�������Ӷ���
class CNetConnection : public INetConnection, public CWinsock
{
public:
	CNetConnection();
	~CNetConnection();

	//�̳� INetConnection
public:
	//��������
	virtual bool Send(const void* pData, uint nDataLen) override;

	//�̳� CWinsock
protected:
	//�������֪ͨ
	//���� pIORequst��IO�����
	virtual void OnIOCompleted(IIORequst* pIORequst) override;

	//���ܺ���
public:
	//���ݳ�ʼ��
	void Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler);
	//��������
	void Clear();
	//��������
	bool Recv();

	//�첽����¼�
protected:
	//���յ��µ�����
	void OnIOCompletedRecv(CNetRecvIOReq* pIORequst);

	//�ڲ�����
protected:
	//�¼�������
	INetConnectionHandler* m_pConnectionHandler;
	//���ջ���	
	WSABUF m_wsaBufRecv;
	char m_bufRecv[NetConstant::kMaxWSABufSize];
	bool m_bRecving;
};

