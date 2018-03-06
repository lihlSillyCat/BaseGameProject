/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: SharedDefine.h
* Author: �����
* Version: 1.0
* Date: 2018/01/25
* Description: �ں˹��̹����壬��������ϵͳ�Ĺ��Զ��塣

* History:
<author>	<time>			 <version>		<desc>
lihl		2018/01/25   	    1.0		    build this module
************************************************************************/
#pragma once

#define safe_close(handle)		if (INVALID_HANDLE_VALUE != (handle)) {::CloseHandle(handle); (handle) = INVALID_HANDLE_VALUE;}
#define safe_release(ptr)		if (nullptr != (ptr)) {(ptr)->Release(); (ptr) = nullptr;}
#define safe_free(ptr)			if (nullptr != (ptr)) {free(ptr); (ptr) = nullptr;}

////////////////////////////////////////////////////////////////////////////
//��������

//��Ч���
inline bool ValidHandle(HANDLE handle)
{
	return (INVALID_HANDLE_VALUE != handle && NULL != handle);
}

//��Ч���
inline bool InvalidHandle(HANDLE handle)
{
	return (INVALID_HANDLE_VALUE == handle || NULL == handle);
}
