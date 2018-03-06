#include "stdafx.h"
#include "NetConnection.h"

CNetConnection::CNetConnection():
	m_bRecving(false)
{
	m_wsaBufRecv.len = array_size(m_bufRecv);
	m_wsaBufRecv.buf = m_bufRecv;
}

CNetConnection::~CNetConnection()
{
}

//发送数据
bool CNetConnection::Send(const void* pData, uint nDataLen)
{
	return true;
}

//操作完成通知
//参数 pIORequst：IO请求包
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

//接收到新的连接
void CNetConnection::OnIOCompletedRecv(CNetRecvIOReq* pIORequst)
{
	//通知处理器
	m_pConnectionHandler->OnRecv(this, m_bufRecv, pIORequst->uNumOfBytesTransferred);
}

//数据初始化
void CNetConnection::Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler)
{
	m_socket = sock;
	m_sockaddr = addr;
	m_pConnectionHandler = pConnectionHandler;
}

//清理数据
void CNetConnection::Clear()
{
	CloseSocket(m_socket);
}

//接收数据
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
