/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: iocp.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/12/26
* Description:封装完成端口

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/26		   1.0		  build this module
lihl		2018/01/25		   1.1		  完善函数的封装和实现，接入日志输出
************************************************************************/
#pragma once

//完成端口类
class CIoCP
{
	//静态函数(直接调用)
public:
	//获取合适的线程数
	static uint GetPreferredThreadsNum();

public:
	CIoCP();
	virtual ~CIoCP();

	//功能函数
public:
	//创建完成端口
	//参数nNumThreads：最大并发线程数
	bool Create(uint nNumThreads);

	//设备关联
	//参数：CompletionKey，用户自定义KEY
	bool AssociateDevice(HANDLE hDevice, ULONG_PTR CompletionKey);

	//等待设备I/O完成，线程阻塞
	//输出参数 lpNumberOfBytes：IO已传输的字节数
	//输出参数 lpCompletionKey：用户在AssociateDevice时传入的KEY
	//输出参数 lpOverlapped：用户在使用异步时传入的Overlapped
	bool GetStatus(LPDWORD lpNumberOfBytes, PULONG_PTR lpCompletionKey, LPOVERLAPPED *lpOverlapped);

	//主动投递I/O完成消息，会触发函数 GetStatus 返回
	//输入参数 dwNumberOfBytesTransferred：函数GetStatus返回该值
	//输入参数 CompletionKey：函数GetStatus返回该值
	//输入参数 lpOverlapped：函数GetStatus返回该值
	bool PostStatus(DWORD dwNumberOfBytesTransferred, ULONG_PTR CompletionKey, LPOVERLAPPED lpOverlapped);

	//释放完成端口
	void Release();

	//内部函数
protected:

	//成员变量
protected:
	//完成端口句柄
	HANDLE	m_hIoCP;
	//线程池线程数
	uint	m_nThreadsNum;
};

