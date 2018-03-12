/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: UserNetClient.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/9
* Description:玩家的网络连接代理

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/9    	   1.0		  build this module
************************************************************************/

#pragma once

#include "KernelLib\ICommunication.h"

class CUserNetClient : public INetConnectionHandler
{
public:
	CUserNetClient();
	~CUserNetClient();

	//继承 INetConnectionHandler
protected:
	//收到数据
	void OnRecv(INetConnection* pConnection, const void* pData, uint nDataLen) override;
	//网络错误
	void OnError(INetConnection* pConnection, ulong errCode) override;
	//断开连接
	void OnDisconnected(INetConnection* pConnection, sint nReason) override;
	//发送完成
	//参数：nSerialNumber 是发送函数Send的返回值
	void OnSent(INetConnection* pConnection, uint nSerialNumber) override;

	//功能函数
public:
	//初始化
	bool Init(INetConnection* pConnection);

	//内部数据
protected:
	//网络连接
	INetConnection* m_pConnection;
};

