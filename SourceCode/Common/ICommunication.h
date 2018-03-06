/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: ICommunication.h
* Author: �����
* Version: 1.0
* Date: 2018/01/02
* Description:ͨ�ŷ���ģ�飬��������ͨ�ţ�TCP��UDP��HTTPS�ȣ�������ͨ�ţ��ܵ��������ڴ桢�ʼ��۵ȣ�

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/01/02		   1.0		  build this module
************************************************************************/

#pragma once

#include "SharedDefine.h"

//����ͨ��Э��
//Note:Ŀǰֻ֧��TCPЭ��
enum NetProtocol
{
	kTCP,	//TCPЭ��
	kUDP,	//UDPЭ��
	//kHttp,	//httpЭ��
	//kHttps, //httpsЭ��
	//kPIPE,	//�ܵ�ͨ��
};

//���������
enum NetError
{
	kDisconnected,	//����Ͽ�
};

//���ӽӿ�����
interface IConnection;
interface IConnectionClient;
interface IConnectionServer;

//������Ƚӿ�
interface IConnectionHandler
{
	//�յ�����
	virtual void OnRecv(IConnection* pConnection, const void* pData, uint nDataLen) = 0;
	//�������
	virtual void OnError(IConnection* pConnection, NetError enError) = 0;
	//�Ͽ�����
	virtual void OnDisconnected(IConnection* pConnection, sint nReason) = 0;
};

//�����������Ƚӿ�
interface IConnectionServerHandler : public IConnectionHandler
{
	//�յ��ͻ�������
	virtual void OnAccept(IConnectionClient* pConnection) = 0;
};

//�ͻ���������Ƚӿ�
interface IConnectionClientHandler : public IConnectionHandler
{
	//�����Ϸ�����
	virtual void OnConnected(IConnectionServer* pConnectionServer) = 0;
};

//�������ӽӿ�
interface IConnection
{
	//��������
	virtual bool Send(const void* pData, uint nDataLen) = 0;
};

//������������ӽӿ�
interface IConnectionServer : public IConnection
{

};

//�ͻ����������ӽӿ�
interface IConnectionClient : public IConnection
{

};

//����ͨ�Žӿ�
interface INetwork
{
	//��ʼ�������
	virtual bool Start(uint nIOThreadsNum) = 0;

	//ֹͣ�������
	virtual void Shutdown() = 0;

	//�ͷŶ���
	//ע�⣺���ñ������󲻵���ʹ�øö���Ӧ��ָ����Ϊ�ա�
	virtual void Release() = 0;

	//��ȡ�Ƽ���IO�߳���
	virtual uint GetPreferredNumIOThreads() = 0;

	//�������������
	//������uPort��Χ [1024,49151]
	virtual IConnectionServer* CreateServer(ushort uPort, NetProtocol enProtocol, IConnectionServerHandler* pHandler) = 0;

	//��������ͻ���
	//������wsIPAddress ������IP�� uPort �������˿�
	virtual IConnectionClient* CreateClient(wchar* wsIPAddress, ushort uPort, NetProtocol enProtocol, IConnectionClientHandler* pHandler) = 0;
};

//��������(��������)
interface INetConnection : public IConnection
{
	//��ַ��Ϣ(ip�Ͷ˿�)
	//virtual bool RemoteAddr(ulong& ip, ushort& port) = 0;
	//virtual bool RemoteAddr(wchar* wsIP, ushort& port) = 0;
	//virtual bool RemoteAddr(wchar* wsIP, wchar* wsPort) = 0;

};

//��������
interface INetServer
{

};

//����ͻ���
interface INetClient : public INetConnection
{

};

//�������ӵ��Ƚӿ�
interface INetConnectionHandler : public IConnectionHandler
{

};

//�������˵��Ƚӿ�
interface INetServerHandler : public IConnectionHandler
{
	//�յ��ͻ�������
	virtual void OnAccept(INetConnection* pConnection) = 0;
};

//����ͨ�ŷ���ӿ�
interface INetworkService
{
	//��ʼ�������
	virtual bool Start() = 0;

	//ֹͣ�������
	//ע�⣺���ñ������󲻵���ʹ�øö���Ӧ��ָ����Ϊ�ա�
	//�����Ҫ�ٴ�ʹ������ͨ�ŷ����������´�������
	virtual void Shutdown() = 0;

	//�ɷ���״̬
	virtual bool Serviceable() = 0;

	//�������������
	//������pConnectionHandler����������������пͻ������ӵ��¼�������
	//������uPort��Χ [1024,49151], ����0��ʾϵͳ����ָ���˿�
	//˵����Ŀǰ��֧��TCP
	virtual INetServer* CreateServer(INetServerHandler* pServerHandler, INetConnectionHandler* pConnectionHandler,
		ushort uPort = 0, NetProtocol enProtocol = NetProtocol::kTCP) = 0;

	//��������ͻ���
	//virtual bool CreateClient

	//�����Ѵ����Ķ��󣨷��������ͻ��ˣ�
	//�û��������û�������˽��ɾ��������������������������ģ��ͳһ����
	//virtual void
};
