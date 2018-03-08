/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: NetworkService.h
* Author: �����
* Version: 1.0
* Date: 2018/2/5
* Description:����ͨ�ŷ��񣬹�������������ͨ�ŷ����ɱ�ģ���ṩ��

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
************************************************************************/
#pragma once

#include <vector>
#include "TCPClient.h"
#include "TCPServer.h"

//�������
//�û��������ֻ�ܴ���һ�������ṩ�����������
class CNetworkService : public INetworkService
{
public:
	CNetworkService();
	~CNetworkService();

	//�̳� INetworkService
public:
	//�������������
	//������pConnectionHandler����������������пͻ������ӵ��¼�������
	//������uPort��Χ [1024,49151], ����0��ʾϵͳ����ָ���˿�
	//˵����Ŀǰ��֧��TCP
	INetServer* CreateServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
		ushort uPort = 0, NetProtocol enProtocol = NetProtocol::kTCP, bool bEnableEnDecryption = true) override;

	//���ܺ���
public:
	//��ʼ�������
	bool Start();
	//ֹͣ�������
	//ע�⣺���ñ������󲻵���ʹ�øö���Ӧ��ָ����Ϊ�ա�
	//�����Ҫ�ٴ�ʹ������ͨ�ŷ����������´�������
	void Shutdown();
	//�ɷ���״̬
	bool Serviceable();

	//���ܺ���
public:

	//�ڲ�����
protected:
	//����TCP����˶���
	INetServer* CreateTCPServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
		ushort uPort, bool bEnableEnDecryption);

	//�ڲ�����
private:
	//ϵͳ�������ݣ�������Ψһ��
	WSADATA m_wsaData;
	//����״̬
	bool m_bRunning;
	//�������б�
	std::vector<CTCPServer*> m_vecTCPServers;
};

