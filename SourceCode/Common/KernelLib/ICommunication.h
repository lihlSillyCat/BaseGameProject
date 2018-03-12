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

#include "KernelModuleDef.h"
#include "IAppInfo.h"
#include "ITrace.h"

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

//���ݷ��ͱ�־
//�ɶ��־��|������ʹ��
enum SendFlag
{
	kNormal				= 0x0001,					//��������
	//Ӧ�ó����Ͱ��������ջ�ϣ���new���ڴ������ʹ�ô˱�־������һ���ڴ濽�����Ч�ʡ�
	//�ر�ע�⣺�ڷ��͹�����Ӧ�ó��򲻵ò������ڴ棬ֱ�����ͳɹ�֪ͨ��ſ��Բ�����
	kDisableBuffer		= (kNormal << 1),			//���û���
	KUrgentData			= (kDisableBuffer << 1),	//�������ݣ����Ŷӣ�ֱ�Ӳ��뷢�Ͷ�����ǰ����
};

//�Ͽ����ӵ�ԭ��
enum DisconnectedCode
{
	//�ͻ��������Ͽ�
	//Э�����Ͽ�
	//�����������Ͽ�
	//����ϵͳԭ��
};

//��������
interface IConnection
{
	//��������
	//ʧ�ܷ���0���ɹ����ر����Ӵ˴η��͵�Ψһ���ͱ�ʶ���������кţ�
	virtual uint Send(const void* pData, uint nDataLen, SendFlag flag = SendFlag::kNormal) = 0;
};

//�����¼�������
template<typename ConnectionImpl>
interface IConnectionHandler
{
	//�յ�����
	virtual void OnRecv(ConnectionImpl* pConnection, const void* pData, uint nDataLen) = 0;
	//�������
	virtual void OnError(ConnectionImpl* pConnection, ulong errCode) = 0;
	//�Ͽ�����
	virtual void OnDisconnected(ConnectionImpl* pConnection, sint nReason) = 0;
	//�������
	//������nSerialNumber �Ƿ��ͺ���Send�ķ���ֵ
	virtual void OnSent(ConnectionImpl* pConnection, uint nSerialNumber) = 0;
};

//��������
interface INetConnectionHandler;
interface INetConnection : public IConnection
{
	//��ַ��Ϣ(ip�Ͷ˿�)
	virtual void GetRemoteAddr(ulong& ip, ushort& port) = 0;
	virtual void GetRemoteAddr(wchar* wsIP, uint nSize, ushort& port) = 0;
	
	//�ӽ���״̬
	//����false��ʾ���봫�䣻����true��ʾ���ܴ���
	virtual bool EnableEnDecryption() = 0;
	
	//�������ӵ��¼�����
	//����������Ĭ��ΪCreateServerʱ�����ͳһ�¼�������
	virtual bool SetHandler(INetConnectionHandler* pHandler) = 0;
	virtual INetConnectionHandler* Handler() = 0;
	
	//�ͷŶ���
	virtual void Release() = 0;
};

//��������
interface INetServer
{
	//��ַ��Ϣ(ip�Ͷ˿�)
	virtual void GetLocalAddr(ulong& ip, ushort& port) = 0;
	virtual void GetLocalAddr(wchar* wsIP, uint nSize, ushort& port) = 0;
	//�ӽ���״̬
	//����false��ʾ���봫�䣻����true��ʾ���ܴ���
	virtual bool EnableEnDecryption() = 0;
};

//����ͻ���
interface INetClient : public INetConnection
{

};

//�������ӵ��Ƚӿ�
interface INetConnectionHandler : public IConnectionHandler<INetConnection>
{

};

//�������˵��Ƚӿ�
interface INetServerHandler
{
	//�յ��ͻ�������
	virtual void OnAccept(INetConnection* pConnection) = 0;
	//�������
	virtual void OnError(ulong errCode) = 0;
};

//����ͨ�ŷ���ӿ�
//���ڱ�ģ�鴴�������Ķ��󲻵����û�������˽�� delete����������������������ģ��ͳһ����
interface INetworkService
{
	//�������������
	//������uPort��Χ [1024,49151], ����0��ʾϵͳ����ָ���˿�
	//˵����Ŀǰ��֧��TCP
	virtual INetServer* CreateServer(INetServerHandler* pServerHandler, ushort uPort = 0, NetProtocol enProtocol = NetProtocol::kTCP, bool bEnableEnDecryption = true) = 0;

	//�ͷ���������


	//��������ͻ���
	//virtual bool CreateClient

	//�����Ѵ����Ķ��󣨷��������ͻ��ˣ�
	//virtual void


};

//ͨ�ŷ���ģ��
interface IProactor;
interface ICommunicationService : public IService
{
	//��ʼ�������
	virtual bool Start(ITrace* pTrace, IProactor* pProactor, DevelopmentEnvironment DevEnvironment) = 0;

	//�汾��Ϣ
	virtual const AppVersion& Version() = 0;

	//��ȡ�������
	//ʧ�ܷ��� nullptr; �ɹ��򷵻��������ɹ�������������
	virtual INetworkService* GetNetworkService() = 0;
};
