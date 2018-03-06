#pragma once

#include "Winsock.h"
#include "NetIORequst.h"

//网络连接对象
class CNetConnection : public INetConnection, public CWinsock
{
public:
	CNetConnection();
	~CNetConnection();

	//继承 INetConnection
public:
	//发送数据
	virtual bool Send(const void* pData, uint nDataLen) override;

	//继承 CWinsock
protected:
	//操作完成通知
	//参数 pIORequst：IO请求包
	virtual void OnIOCompleted(IIORequst* pIORequst) override;

	//功能函数
public:
	//数据初始化
	void Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler);
	//清理数据
	void Clear();
	//接收数据
	bool Recv();

	//异步完成事件
protected:
	//接收到新的连接
	void OnIOCompletedRecv(CNetRecvIOReq* pIORequst);

	//内部数据
protected:
	//事件处理器
	INetConnectionHandler* m_pConnectionHandler;
	//接收缓存	
	WSABUF m_wsaBufRecv;
	char m_bufRecv[NetConstant::kMaxWSABufSize];
	bool m_bRecving;
};

