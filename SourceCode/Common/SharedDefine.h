/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: SharedDefine.h
* Author: 李辉龙  
* Version: 1.0
* Date: 2017/10/17
* Description: 工程共享定义，该头文件需优先包含。

* History:
	<author>	<time>			 <version>		<desc>	
	lihl		2017/10/17		    1.0		    build this module 	
	lihl		2018/01/25			1.1			去掉操作系统相关项
************************************************************************/
#pragma once

/********************* 系统说明 *********************/
/* 
* 1，禁止在公共定义和上层框架中包含操作系统相关项，只有内核才与操作系统相关。
  除内核外，均使用C++11及以上版本标准。涉及跨平台只需要更换内核。
* 2，整套系统使用 Unicode 字符集。计算机处理 Unicode 比 ANSI 高效。
* 3，整套系统优先使用64位操作系统，非不得已才用X86。
* 4，保持代码风格统一。
*/

////////////////////////////////////////////////////////////////////////////
//类型定义

//Note:计算机处理无符号数字比处理有符号数字快。
//不要在和其他端的消息协议中使用 bool 类型， 因为不同编译器和不同机器对于 bool 类型所占字节数处理不同。

//常用类型定义
typedef unsigned char			uchar;
typedef	signed char				schar;
typedef	unsigned short			ushort;
typedef	signed short			sshort;
typedef	unsigned int			uint;
typedef	signed int				sint;
typedef	unsigned long			ulong;
typedef	signed long				slong;
typedef	unsigned long long		ullong;
typedef	signed long long		sllong;
typedef wchar_t					wchar;

//类型按字节定义
typedef unsigned __int8			uint8;
typedef	signed	 __int8			sint8;
typedef	unsigned __int16		uint16;
typedef	signed	 __int16		sint16;
typedef	unsigned __int32		uint32;
typedef	signed   __int32		sint32;
typedef	unsigned __int64		uint64;
typedef	signed   __int64		sint64;

////////////////////////////////////////////////////////////////////////////
//语句宏定义（采用C++关键字风格）

//在combaseapi.h文件中有定义interface（等同struct）
#ifdef interface
#undef interface
#endif
#define interface struct //接口定义 

//释放内存
#define safe_delete(ptr)		if (nullptr != (ptr)) {delete (ptr); (ptr) = nullptr;}
#define safe_delete_arr(ptr)	if (nullptr != (ptr)) {delete[] (ptr); (ptr) = nullptr;}

//数组长度 (chrome 源代码，模板含义：返回数组长度的函数)
template <typename T, size_t N>
char(&ArraySizeHelper(T(&array_name)[N]))[N];
#define array_size(array_name) (sizeof(ArraySizeHelper(array_name)))

////////////////////////////////////////////////////////////////////////////
//类相关宏定义

// 禁止拷贝构造和赋值
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

//////////////////////////////////////////////////////////////////////////
//异常捕获（DEBUG模式直接崩溃方便查错调试，RELEASE模式捕获异常）


//end file
///////////////////////////////////////////////////////////////////////////