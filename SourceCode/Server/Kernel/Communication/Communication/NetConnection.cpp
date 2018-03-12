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

//发送数据
//发送数据
//失败返回0；成功返回本连接此次发送的唯一发送标识（发送序列号）
uint CNetConnection::Send(const void* pData, uint nDataLen, SendFlag flag)
{
	return 0;
}

//地址信息(ip和端口)
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

//设置连接的事件处理
//若不设置则默认为CreateServer时传入的统一事件处理器
bool CNetConnection::SetHandler(INetConnectionHandler* pHandler)
{
	if (nullptr == pHandler)
	{
		return false;
	}

	m_pConnectionHandler = pHandler;

	return true;
}

//释放对象
void CNetConnection::Release()
{
	Clear();

	NetConnectionPool()->Recycle(this);
}

//操作完成通知
//参数 pIORequst：IO请求包
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

//操作完成通知（失败）
//参数 pIORequst：IO请求包
//参数 nErrorCode ： 错误码
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

//接收到新的连接
void CNetConnection::OnIOCompletedRecv(CNetRecvIOReq* pIORequst)
{
	//通知处理器
	m_pConnectionHandler->OnRecv(this, m_bufRecv, pIORequst->uNumOfBytesTransferred);

	delete pIORequst;
}

void CNetConnection::OnIOCompletedRecvError(CNetRecvIOReq* pIORequst, ulong nErrorCode)
{
	//通知处理器
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

//数据初始化
void CNetConnection::Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler, bool bEncryptionDecryption)
{
	m_socket = sock;
	m_sockaddr = addr;
	m_pConnectionHandler = pConnectionHandler;
	m_bEncryptionDecryption = bEncryptionDecryption;
}

//开始工作
bool CNetConnection::Start()
{
	m_bRunning = true;

	return RecvData();
}

//清理数据
void CNetConnection::Clear()
{
	CloseSocket(m_socket);
}

//接收数据
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
