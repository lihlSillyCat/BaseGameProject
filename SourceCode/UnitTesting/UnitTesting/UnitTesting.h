/************************************************************************
* Copyright(c) 2017-2037 ���ι�˾
* All rights reserved.
*
* license
*
* FileName: UnitTesting.h
* Author: �����
* Version: 1.0
* Date: 2017/10/20
* Description:��Ԫ�����࣬���ڲ��������������й��ܼ��ȶ��Ժ�Ч�ʵȡ�

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/20			1.0		  build this module
************************************************************************/
#pragma once

enum UnitTestingItem
{
	kMinTest,
	kUTMacros,	//����������
	kUTThreadReadWrite,	//һ���߳�д��һ���̶߳�
	kVirtualFunction, //�麯����
	kC2W,
	kReverse,		  //����������ת���������ݿ��麯�����Ƿ�����
	kNewDel,
	kUTMax,
};

class CUnitTesting
{
public:
	CUnitTesting();
	~CUnitTesting();

public:
	//ִ�е�Ԫ����
	void UnitTest(UnitTestingItem item);
	
	//����ִ�к���
private:
	void MinTest();
	//�����
	void UnitTestMacros();
	//һ���߳�д��һ���̶߳�
	void UnitTestThreadReadWrite();
	//�麯����
	void VirtualFunctionPtr();
	
	void CharToWchar();
	//����������ת���������ݿ��麯�����Ƿ�����
	void ReverseClass();

	void NewDel();

private:
	//������
	typedef void(CUnitTesting::*ExecFunction)(void);
	static ExecFunction	m_FuncMap[UnitTestingItem::kUTMax];
};

