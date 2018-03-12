#pragma once

#include "Winsock.h"
#include "NetIORequst.h"

//网络连接对象
class CNetConnection : public CWinsock, public INetConnection
{
public:
	CNetConnection();
	~CNetConnection();

	//继承 INetConnection
public:
	//发送数据
	//失败返回0；成功返回本连接此次发送的唯一发送标识（发送序列号）
	uint Send(const void* pData, uint nDataLen, SendFlag flag = SendFlag::kNormal) override;
	//地址信息(ip和端口)
	void GetRemoteAddr(ulong& ip, ushort& port) override;
	void GetRemoteAddr(wchar* wsIP, uint nSize, ushort& port) override;
	//加解密状态
	//返回false表示明码传输；返回true表示加密传输
	bool EnableEnDecryption() override { return m_bEncryptionDecryption; }
	//设置连接的事件处理
	//若不设置则默认为CreateServer时传入的统一事件处理器
	bool SetHandler(INetConnectionHandler* pHandler) override;
	INetConnectionHandler* Handler() override { return m_pConnectionHandler; }
	//释放对象
	void Release() override;

	//继承 CWinsock
protected:
	//操作完成通知
	//参数 pIORequst：IO请求包
	void OnIOCompleted(IIORequst* pIORequst) override;
	//操作完成通知（失败）
	//参数 pIORequst：IO请求包
	//参数 nErrorCode ： 错误码
	void OnIOCompletedError(IIORequst* pIORequst, ulong nErrorCode) override;

	//功能函数
public:
	//数据初始化
	void Init(SOCKET sock, sockaddr_in& addr, INetConnectionHandler* pConnectionHandler, bool bEncryptionDecryption);
	//开始工作
	bool Start();
	//清理数据
	void Clear();

protected:
	//接收数据
	bool RecvData();

	//异步完成事件
protected:
	//接收到新的连接
	void OnIOCompletedRecv(CNetRecvIOReq* pIORequst);
	void OnIOCompletedRecvError(CNetRecvIOReq* pIORequst, ulong nErrorCode);

	//内部数据
protected:
	//事件处理器
	INetConnectionHandler* m_pConnectionHandler;
	//接收缓存	
	WSABUF m_wsaBufRecv;
	char m_bufRecv[NetConstant::kMaxWSABufSize];
	//加解密
	bool m_bEncryptionDecryption;
};

