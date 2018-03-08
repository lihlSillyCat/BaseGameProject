/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: UIService.h
* Author: �����
* Version: 1.0
* Date: 2017/12/21
* Description:�������������

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/21		   1.0		  build this module
lihl		2018/3/5		   1.0		  ��д�������UI����
************************************************************************/
#pragma once

#include <unordered_map>

enum UICommand
{
	kStart,			//��ʼ
	kShutdown,		//�رշ�����
	kUpdateConfig,	//��������
	kHelp,			//����
	kInvalid,		//��Ч����	
	//...			//��ʱ���
};

//��������
struct UICmdData
{
	UICommand cmd;		//����
	schar param[256];	//�Զ������
};

class CUIService : public ITraceView
{
public:
	CUIService();
	~CUIService();

	//�̳� ITraceView
public:
	//�������
	void PrintOnView(const wchar* wsMsg, ulong color = 0) override;

	//�������
public:
	//��ʼ
	bool Start();
	//����
	void Shutdown();
	//�ɷ���״̬
	bool Servable() const { return m_bRunning; }

	//���ܺ���
public:
	//����GUI֡ˢ��
	//������Ч�򷵻�false
	bool CanvasUpdate(UICmdData &Cmd);
	//׷����ʾ����
	bool AppendMsg(const wchar* wsMsg);
	//��ʾ����
	void ShowHelp();

	//�ڲ�����
protected:

	//��Ա����
private:
	//����״̬
	bool m_bRunning;
	//�����
	const std::unordered_map<std::string, UICommand> m_CmdLibraries;
};

