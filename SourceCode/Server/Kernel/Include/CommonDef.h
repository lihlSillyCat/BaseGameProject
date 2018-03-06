/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: SharedDefine.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/01/25
* Description: 内核工程共享定义，包含操作系统的共性定义。

* History:
<author>	<time>			 <version>		<desc>
lihl		2018/01/25   	    1.0		    build this module
************************************************************************/
#pragma once

#define safe_close(handle)		if (INVALID_HANDLE_VALUE != (handle)) {::CloseHandle(handle); (handle) = INVALID_HANDLE_VALUE;}
#define safe_release(ptr)		if (nullptr != (ptr)) {(ptr)->Release(); (ptr) = nullptr;}
#define safe_free(ptr)			if (nullptr != (ptr)) {free(ptr); (ptr) = nullptr;}

////////////////////////////////////////////////////////////////////////////
//内联函数

//有效句柄
inline bool ValidHandle(HANDLE handle)
{
	return (INVALID_HANDLE_VALUE != handle && NULL != handle);
}

//无效句柄
inline bool InvalidHandle(HANDLE handle)
{
	return (INVALID_HANDLE_VALUE == handle || NULL == handle);
}
