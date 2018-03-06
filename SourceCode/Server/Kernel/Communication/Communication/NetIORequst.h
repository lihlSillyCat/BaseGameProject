/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: NetIORequst.h
* Author: �����
* Version: 1.0
* Date: 2018/2/27
* Description: �����첽��������

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/27     	   1.0		  build this module
lihl		2018/3/1     	   1.1		  ���Accept���첽IO������
************************************************************************/

#pragma once

#include "Winsock.h"

//�첽��������
enum NetAsyncOperation
{
	kAccept,		//����˽��տͻ�����������
	kConnect,		//�ͻ����������ӷ����
	kRead,			//��ȡ����
	kWrite,			//��������
};

//����IO����
class CNetIORequst : public IIORequst
{
public:
	CNetIORequst(NetAsyncOperation opt);
	~CNetIORequst();

public:
	//IO��������
	NetAsyncOperation NetOperation;
};

//TCP������������AcceptEx��IO�������
class CNetAcceptIOReq : public CNetIORequst
{
public:
	CNetAcceptIOReq();
	~CNetAcceptIOReq();

public:
	//AcceptEx�������ݻ�����(�ֽ���Э�飺FirstBlock + LocalAddress + RemoteAddress)(FirstBlock=0)
	char sDataBuf[NetConstant::kAddrBufSize * 2 + 1];
	
	//��AcceptEx��SOCKET
	SOCKET sockAccept;
};

//TCP������������WSARecv��IO�������
class CNetRecvIOReq : public CNetIORequst
{
public:
	CNetRecvIOReq();
	~CNetRecvIOReq();

public:
	ulong uRecvFlags;
};

