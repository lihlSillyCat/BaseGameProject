// Communication.cpp: 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "Communication.h"


CNetwork::CNetwork(ITrace* pTrace):
	m_pTrace(pTrace),
	m_bRunning(false)
{

}

CNetwork::~CNetwork()
{

}


//开始网络服务
bool CNetwork::Start(uint nIOThreadsNum)
{
	//初始化Windows网络应用
	int res = ::WSAStartup(MAKEWORD(2, 2), &m_wsaData);
	if (0 != res)
	{
		m_pTrace->LogErrorFormat(L"WSAStartup failed:%d", res);
		return false;
	}

	//初始化主动器（Proactor）


	m_bRunning = true;

	return true;
}

//停止网络服务
void CNetwork::Shutdown()
{
	m_bRunning = false;

	::WSACleanup();
}

//释放对象
//注意：调用本函数后不得再使用该对象。应将指针置为空。
void CNetwork::Release()
{
	delete this;
}

//获取推荐的IO线程数
uint CNetwork::GetPreferredNumIOThreads()
{
	return 0;
}

//创建网络服务器
//参数：uPort范围 [1024,49151]
IConnectionServer* CNetwork::CreateServer(ushort uPort, NetProtocol enProtocol, IConnectionServerHandler* pHandler)
{
	if (1024 > uPort || uPort > 49151 || nullptr == pHandler || !m_bRunning)
	{
		return nullptr;
	}


	return nullptr;
}

//创建网络客户端
IConnectionClient* CNetwork::CreateClient(wchar* wsIPAddress, ushort uPort, NetProtocol enProtocol, IConnectionClientHandler* pHandler)
{
	return nullptr;
}
