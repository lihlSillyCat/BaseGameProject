/************************************************************************
* Copyright(c) 2017-2037 做梦公司
* All rights reserved.
*
* license
*
* FileName: UnitTesting.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/10/20
* Description:单元测试类，用于测试整个工程所有功能及稳定性和效率等。

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/20			1.0		  build this module
************************************************************************/
#pragma once

enum UnitTestingItem
{
	kMinTest,
	kUTMacros,	//宏命名测试
	kUTThreadReadWrite,	//一个线程写，一个线程读
	kVirtualFunction, //虚函数表
	kC2W,
	kReverse,		  //将父子类逆转换更改数据看虚函数表是否受损
	kNewDel,
	kUTMax,
};

class CUnitTesting
{
public:
	CUnitTesting();
	~CUnitTesting();

public:
	//执行单元测试
	void UnitTest(UnitTestingItem item);
	
	//具体执行函数
private:
	void MinTest();
	//宏测试
	void UnitTestMacros();
	//一个线程写，一个线程读
	void UnitTestThreadReadWrite();
	//虚函数表
	void VirtualFunctionPtr();
	
	void CharToWchar();
	//将父子类逆转换更改数据看虚函数表是否受损
	void ReverseClass();

	void NewDel();

private:
	//函数表
	typedef void(CUnitTesting::*ExecFunction)(void);
	static ExecFunction	m_FuncMap[UnitTestingItem::kUTMax];
};

