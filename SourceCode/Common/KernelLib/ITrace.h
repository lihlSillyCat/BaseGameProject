/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: ITrace.h
* Author: �����
* Version: 1.0
* Date: 2017/10/17
* Description:��־��

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/17		   1.0		  build this module
************************************************************************/
#pragma once
#include <assert.h>
#include "KernelModuleDef.h"

///////////////////////////////////////////////////////////////////////////////
//������

//Դ���붨λ
#define LOCATE_SC(trace, msg, type)	trace->LogLocate(type, __FILE__, __FUNCTION__, __LINE__, msg);

//���ԣ�RELEASE�汾�����־��DEBUG�汾�����־���ó���ֱ�ӱ�����
#if defined(_DEBUG) || defined(DEBUG)
//�����ж�
#define ASSERT_LOG(trace, expression, msg)	{ if (!(expression)) {LOCATE_SC(trace, msg, kTraceAssertion); assert(false);}} 
//���Դ���
#define ASSERT_FALSE(trace, msg)	{LOCATE_SC(trace, msg, kTraceAssertion); assert(false);}
#else
//�����ж�
#define ASSERT_LOG(trace, expression, msg)	{ if (!(expression)) LOCATE_SC(trace, msg, kTraceAssertion);} 
//���Դ���
#define ASSERT_FALSE(trace, msg)	LOCATE_SC(trace, msg, kTraceAssertion)
#endif

//��־�ȼ����ӵ͵��ߣ�
enum TraceLevel
{
	kTraceLog,			//��־
	kTraceDebug,		//DEBUG��־
	kTraceInformation,	//��Ϣ
	kTraceWarning,		//����
	kTraceAssertion,	//����
	kTraceError,		//����
	kTraceCritical,		//�ٽ�㡢�ؼ�Σ����
	kTraceException,	//�쳣
	kTraceCustomize,	//�û��Զ���
	kTraceLevelNum,		//�ȼ����������û�ʹ�ã�
};

//��־��ӿ�(�̰߳�ȫ)
//ֻ�ṩ UNICODE �汾 (���������UNICODE�ַ�����ANSI�ַ�����Ч�ʸ�)
interface ITrace
{
	//��־���
	virtual void Log(TraceLevel enLevel, const wchar* wsMsg) = 0;
	virtual void LogFormat(TraceLevel enLevel, const wchar* wsFormat, ...) = 0;

	//��ӡ��־
	virtual void Log(const wchar* wsMsg) = 0;
	virtual void LogFormat(const wchar* wsFormat, ...) = 0;

	//��ӡDEBUG��־
	virtual void LogDebug(const wchar* wsMsg) = 0;
	virtual void LogDebugFormat(const wchar* wsFormat, ...) = 0;

	//��ӡ��Ϣ
	virtual void LogInfo(const wchar* wsMsg) = 0;
	virtual void LogInfoFormat(const wchar* wsFormat, ...) = 0;

	//��ӡ����
	virtual void LogWarning(const wchar* wsMsg) = 0;
	virtual void LogWarningFormat(const wchar* wsFormat, ...) = 0;

	//��ӡ����
	virtual void LogAssertion(const wchar* wsMsg) = 0;
	virtual void LogAssertionFormat(const wchar* wsFormat, ...) = 0;

	//��ӡ����
	virtual void LogError(const wchar* wsMsg) = 0;
	virtual void LogErrorFormat(const wchar* wsFormat, ...) = 0;

	//��ӡ�ؼ�Σ��
	virtual void LogCritical(const wchar* wsMsg) = 0;
	virtual void LogCriticalFormat(const wchar* wsFormat, ...) = 0;

	//��ӡ�쳣
	virtual void LogException(const wchar* wsMsg) = 0;
	virtual void LogExceptionFormat(const wchar* wsFormat, ...) = 0;

	//��ӡ�Զ�����Ϣ
	virtual void LogCustom(ulong color, const wchar* wsMsg) = 0;
	virtual void LogCustomFormat(ulong color, const wchar* wsFormat, ...) = 0;

	//���붨λ
	virtual void LogLocate(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsMsg) = 0;
	virtual void LogLocateFormat(TraceLevel enLevel, const wchar* wsFileName, const wchar* wsFuncName, uint uLineCnt, const wchar* wsFormat, ...) = 0;
	//���붨λ��__FILE__,__LINE__ Ϊchar, �˴��ṩchar�汾��
	virtual void LogLocate(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsMsg) = 0;
	virtual void LogLocateFormat(TraceLevel enLevel, const char* strFileName, const char* strFuncName, uint uLineCnt, const wchar* wsFormat, ...) = 0;
};

//��־������ͼ
interface ITraceView
{
	//�������
	virtual void PrintOnView(const wchar* wsMsg, ulong color = 0) = 0;
};

//��־������
interface ITraceService : public ITrace , public IService
{
	//��������
	//������pView������ͼ
	//������wsLogFileName����־�ļ���
	virtual bool Start(ITraceView* pView, const wchar* wsLogFileName) = 0;
};
