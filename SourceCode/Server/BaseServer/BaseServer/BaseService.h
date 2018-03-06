/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: BaseService.h
* Author: �����
* Version: 1.0
* Date: 2018/3/5
* Description:���������࣬��Ӧ�ò��ṩ�ײ�֧��

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/5    	   1.0		  build this module
************************************************************************/

#pragma once

#include "ImportExport\TraceModule.h"
#include "ImportExport\CommonModule.h"
#include "UIService.h"
#include "AppInfo.h"

//������������Ϣ����ģʽ��������ѭ��ˢ��
class CBaseService
{
public:
	CBaseService();
	~CBaseService();

	//�������
public:
	//��ʼ
	bool Start();
	//����
	void Shutdown();
	//��ѭ��
	void Update();
	//�ɷ���״̬
	bool Servable() const { return m_bRunning; }

	//�������
public:
	//��־����
	ITrace* Trace() { return m_TraceHelper.GetTrace(); }

	//���ܺ���
public:

	//��Ա����
private:
	bool m_bRunning;	//����״̬
	
	//�������
private:
	//Ӧ�ó�����Ϣ
	CAppInfo m_AppInfo;
	//UI����
	CUIService m_UIService;
	//���ݷ���
	//....
	//�������
	CCommonHelper m_CommonHelper;
	IProactor* m_pProactor;
	//��־����
	CTraceHelper m_TraceHelper;
};

