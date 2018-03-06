#pragma once

#include "ICommunication.h"

class CNetwork : public INetwork
{
public:
	CNetwork(ITrace* pTrace);
	~CNetwork();

	//INetwork继承
public:
	//开始网络服务
	bool Start(uint nIOThreadsNum) override;

	//停止网络服务
	void Shutdown() override;

	//释放对象
	//注意：调用本函数后不得再使用该对象。应将指针置为空。
	void Release() override;

	//获取推荐的IO线程数
	uint GetPreferredNumIOThreads() override;

	//创建网络服务器
	//参数：uPort范围 [1024,49151]
	IConnectionServer* CreateServer(ushort uPort, NetProtocol enProtocol, IConnectionServerHandler* pHandler) override;

	//创建网络客户端
	IConnectionClient* CreateClient(wchar* wsIPAddress, ushort uPort, NetProtocol enProtocol, IConnectionClientHandler* pHandler) override;

	//成员变量
protected:
	ITrace*	m_pTrace;
	WSADATA	m_wsaData;	//系统网络数据
	bool m_bRunning;
};
