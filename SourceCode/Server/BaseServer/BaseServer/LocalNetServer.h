/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: LocalNetServer.h
* Author: �����
* Version: 1.0
* Date: 2018/3/8
* Description:�������������

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

	//�̳� INetServerHandler
protected:

	//�̳� INetConnectionHandler
protected:

private:

};

