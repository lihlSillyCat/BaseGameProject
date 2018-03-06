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
************************************************************************/

#include "stdafx.h"
#include "NetIORequst.h"


CNetIORequst::CNetIORequst(NetAsyncOperation opt):
	NetOperation(opt)
{

}

CNetIORequst::~CNetIORequst()
{

}

///////////////////////////////////////////////////////////////////
////TCP������������AcceptEx��IO�������
CNetAcceptIOReq::CNetAcceptIOReq() : CNetIORequst(NetAsyncOperation::kAccept)
{

}

CNetAcceptIOReq::~CNetAcceptIOReq()
{

}

//TCP������������WSARecv��IO�������
CNetRecvIOReq::CNetRecvIOReq() : 
	CNetIORequst(NetAsyncOperation::kRead),
	uRecvFlags(0)
{

}

CNetRecvIOReq::~CNetRecvIOReq()
{

}
