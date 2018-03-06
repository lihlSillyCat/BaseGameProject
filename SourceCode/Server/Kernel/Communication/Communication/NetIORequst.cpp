/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: NetIORequst.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/27
* Description: 网络异步操作请求

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
////TCP网络服务端用于AcceptEx的IO请求对象
CNetAcceptIOReq::CNetAcceptIOReq() : CNetIORequst(NetAsyncOperation::kAccept)
{

}

CNetAcceptIOReq::~CNetAcceptIOReq()
{

}

//TCP网络服务端用于WSARecv的IO请求对象
CNetRecvIOReq::CNetRecvIOReq() : 
	CNetIORequst(NetAsyncOperation::kRead),
	uRecvFlags(0)
{

}

CNetRecvIOReq::~CNetRecvIOReq()
{

}
