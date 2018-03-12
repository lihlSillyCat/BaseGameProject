#include "stdafx.h"
#include "NetConnection.h"

CNetConnection::CNetConnection():
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

//�������ӵ��¼�����
//����������Ĭ��ΪCreateServerʱ�����ͳһ�¼�������
bool CNetConnection::SetHandler(INetConnectionHandler* pHandler)
{
	if (nullptr == pHandler)
	{
		return false;
	}

	m_pConnectionHandler = pHandler;

	return true;
}

//�ͷŶ���
void CNetConnection::Release()
{
	Clear();

	NetConnectionPool()->Recycle(this);
}

//�������֪ͨ
//���� pIORequst��IO�����
void CNetConnection::OnIOCompleted(IIORequst* pIORequst)
{
	switch (((CNetIORequst*)pIORequst)->NetOperation)
	{
	case NetAsyncOperation::kRead:
		return OnIOCompletedRecv((CNetRecvIOReq*)pIORequst);
	default:
		break;
	}
}

//�������֪ͨ��ʧ�ܣ�
//���� pIORequst��IO�����
//���� nErrorCode �� ������
void CNetConnection::OnIOCompletedError(IIORequst* pIORequst, ulong nErrorCode)
{
	switch (((CNetIORequst*)pIORequst)->NetOperation)
	{
	case NetAsyncOperation::kRead:
		return OnIOCompletedRecvError((CNetRecvIOReq*)pIORequst, nErrorCode);
	default:
		break;
	}
}

//���յ��µ�����
void CNetConnection::OnIOCompletedRecv(CNetRecvIOReq* pIORequst)
{
	//֪ͨ������
	m_pConnectionHandler->OnRecv(this, m_bufRecv, pIORequst->uNumOfBytesTransferred);

	delete pIORequst;
}

void CNetConnection::OnIOCompletedRecvError(CNetRecvIOReq* pIORequst, ulong nErrorCode)
{
	//֪ͨ������
	if (ERROR_NETNAME_DELETED == nErrorCode)
	{
		m_pConnectionHandler->OnDisconnected(this, nErrorCode);
	}
	else
	{
		m_pConnectionHandler->OnError(this, nErrorCode);
	}
	
	delete pIORequst;
}

//���ݳ�ʼ��
void CNetConnection::Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler, bool bEncryptionDecryption)
{
	m_socket = sock;
	m_sockaddr = addr;
	m_pConnectionHandler = pConnectionHandler;
	m_bEncryptionDecryption = bEncryptionDecryption;
}

//��ʼ����
bool CNetConnection::Start()
{
	m_bRunning = true;

	return RecvData();
}

//��������
void CNetConnection::Clear()
{
	CloseSocket(m_socket);
}

//��������
bool CNetConnection::RecvData()
{
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
