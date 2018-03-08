#include "stdafx.h"
#include "NetConnection.h"

CNetConnection::CNetConnection():
	m_bRecving(false),
	m_bEncryptionDecryption(true)
{
	m_wsaBufRecv.len = array_size(m_bufRecv);
	m_wsaBufRecv.buf = m_bufRecv;
}

CNetConnection::~CNetConnection()
{
}

//��������
//��������
//ʧ�ܷ���0���ɹ����ر����Ӵ˴η��͵�Ψһ���ͱ�ʶ���������кţ�
uint CNetConnection::Send(const void* pData, uint nDataLen, SendFlag flag)
{
	return 0;
}

//��ַ��Ϣ(ip�Ͷ˿�)
void CNetConnection::GetRemoteAddr(ulong& ip, ushort& port)
{
	ip = m_sockaddr.sin_addr.S_un.S_addr;
	port = m_sockaddr.sin_port;
}

void CNetConnection::GetRemoteAddr(wchar* wsIP, uint nSize, ushort& port)
{
	port = m_sockaddr.sin_port;
	InetNtop(m_sockaddr.sin_family, &m_sockaddr.sin_addr, wsIP, nSize);
}

//�������֪ͨ
//���� pIORequst��IO�����
void CNetConnection::OnIOCompleted(IIORequst* pIORequst)
{
	switch (((CNetIORequst*)pIORequst)->NetOperation)
	{
	case NetAsyncOperation::kRead:
		OnIOCompletedRecv((CNetRecvIOReq*)pIORequst);
		break;
	default:
		break;
	}
}

//���յ��µ�����
void CNetConnection::OnIOCompletedRecv(CNetRecvIOReq* pIORequst)
{
	//֪ͨ������
	m_pConnectionHandler->OnRecv(this, m_bufRecv, pIORequst->uNumOfBytesTransferred);
}

//���ݳ�ʼ��
void CNetConnection::Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler, bool bEncryptionDecryption)
{
	m_socket = sock;
	m_sockaddr = addr;
	m_pConnectionHandler = pConnectionHandler;
	m_bEncryptionDecryption = bEncryptionDecryption;
}

//��������
void CNetConnection::Clear()
{
	CloseSocket(m_socket);
}

//��������
bool CNetConnection::Recv()
{
	if (m_bRecving)
	{
		return true;
	}

	CNetRecvIOReq* pReq = new CNetRecvIOReq();
	if (SOCKET_ERROR == WSARecv(m_socket,
		&m_wsaBufRecv, 1,
		&pReq->uNumOfBytesTransferred,
		&pReq->uRecvFlags,
		pReq,
		NULL))
	{
		int err = WSAGetLastError();
		if (WSA_IO_PENDING != err)
		{
			Trace()->LogErrorFormat(L"WSARecv error[%d]", err);
			delete pReq;
			return false;
		}
	}

	return true;
}
