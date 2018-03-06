/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: SharedDefine.h
* Author: �����  
* Version: 1.0
* Date: 2017/10/17
* Description: ���̹����壬��ͷ�ļ������Ȱ�����

* History:
	<author>	<time>			 <version>		<desc>	
	lihl		2017/10/17		    1.0		    build this module 	
	lihl		2018/01/25			1.1			ȥ������ϵͳ�����
************************************************************************/
#pragma once

/********************* ϵͳ˵�� *********************/
/* 
* 1����ֹ�ڹ���������ϲ����а�������ϵͳ����ֻ���ں˲������ϵͳ��ء�
  ���ں��⣬��ʹ��C++11�����ϰ汾��׼���漰��ƽֻ̨��Ҫ�����ںˡ�
* 2������ϵͳʹ�� Unicode �ַ�������������� Unicode �� ANSI ��Ч��
* 3������ϵͳ����ʹ��64λ����ϵͳ���ǲ����Ѳ���X86��
* 4�����ִ�����ͳһ��
*/

////////////////////////////////////////////////////////////////////////////
//���Ͷ���

//Note:����������޷������ֱȴ����з������ֿ졣
//��Ҫ�ں������˵���ϢЭ����ʹ�� bool ���ͣ� ��Ϊ��ͬ�������Ͳ�ͬ�������� bool ������ռ�ֽ�������ͬ��

//�������Ͷ���
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

//���Ͱ��ֽڶ���
typedef unsigned __int8			uint8;
typedef	signed	 __int8			sint8;
typedef	unsigned __int16		uint16;
typedef	signed	 __int16		sint16;
typedef	unsigned __int32		uint32;
typedef	signed   __int32		sint32;
typedef	unsigned __int64		uint64;
typedef	signed   __int64		sint64;

////////////////////////////////////////////////////////////////////////////
//���궨�壨����C++�ؼ��ַ��

//��combaseapi.h�ļ����ж���interface����ͬstruct��
#ifdef interface
#undef interface
#endif
#define interface struct //�ӿڶ��� 

//�ͷ��ڴ�
#define safe_delete(ptr)		if (nullptr != (ptr)) {delete (ptr); (ptr) = nullptr;}
#define safe_delete_arr(ptr)	if (nullptr != (ptr)) {delete[] (ptr); (ptr) = nullptr;}

//���鳤�� (chrome Դ���룬ģ�庬�壺�������鳤�ȵĺ���)
template <typename T, size_t N>
char(&ArraySizeHelper(T(&array_name)[N]))[N];
#define array_size(array_name) (sizeof(ArraySizeHelper(array_name)))

////////////////////////////////////////////////////////////////////////////
//����غ궨��

// ��ֹ��������͸�ֵ
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&);               \
	void operator=(const TypeName&)

//////////////////////////////////////////////////////////////////////////
//�쳣����DEBUGģʽֱ�ӱ�����������ԣ�RELEASEģʽ�����쳣��


//end file
///////////////////////////////////////////////////////////////////////////