/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: LocalNetServer.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/8
* Description:本地网络服务器

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/8    	   1.0		  build this module
************************************************************************/

#pragma once

#include "KernelLib\ICommunication.h"

class CLocalNetServer : public INetServerHandler, public INetConnectionHandler
{
public:
	CLocalNetServer();
	~CLocalNetServer();

	//继承 INetServerHandler
protected:

	//继承 INetConnectionHandler
protected:

private:

};

