#pragma once

#include "Winsock.h"
#include "NetIORequst.h"

//�������Ӷ���
class CNetConnection : public CWinsock, public INetConnection
{
public:
	CNetConnection();
	~CNetConnection();

	//�̳� INetConnection
public:
	//��������
	//ʧ�ܷ���0���ɹ����ر����Ӵ˴η��͵�Ψһ���ͱ�ʶ���������кţ�
	uint Send(const void* pData, uint nDataLen, SendFlag flag = SendFlag::kNormal) override;
	//��ַ��Ϣ(ip�Ͷ˿�)
	void GetRemoteAddr(ulong& ip, ushort& port) override;
	void GetRemoteAddr(wchar* wsIP, uint nSize, ushort& port) override;
	//�ӽ���״̬
	//����false��ʾ���봫�䣻����true��ʾ���ܴ���
	bool EnableEnDecryption() override { return m_bEncryptionDecryption; }

	//�̳� CWinsock
protected:
	//�������֪ͨ
	//���� pIORequst��IO�����
	void OnIOCompleted(IIORequst* pIORequst) override;

	//���ܺ���
public:
	//���ݳ�ʼ��
	void Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler, bool bEncryptionDecryption);
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
	//�ӽ���
	bool m_bEncryptionDecryption;
};

