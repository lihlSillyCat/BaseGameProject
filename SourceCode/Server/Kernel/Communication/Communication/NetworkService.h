/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: NetworkService.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/2/5
* Description:网络通信服务，工程内所有网络通信服务都由本模块提供。

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/2/5     	   1.0		  build this module
************************************************************************/
#pragma once

//网络服务
//用户程序最多只能创建一个对象，提供所有网络服务
class CNetworkService : INetworkService
{
public:
	CNetworkService(ITrace* pTrace);
	~CNetworkService();

	//继承 INetworkService
public:
	//开始网络服务
	bool Start() override;
	//停止网络服务
	//注意：调用本函数后不得再使用该对象。应将指针置为空。
	//如果需要再次使用网络通信服务，则需重新创建对象
	void Shutdown() override;
	//可服务状态
	bool Serviceable() override;

	//功能函数
public:

	//内部数据
private:
	//系统网络数据（工程内唯一）
	WSADATA m_wsaData;
	//日志输出（工程内唯一）
	ITrace* m_pTrace;
	//运行状态
	bool m_bRunning;

};

