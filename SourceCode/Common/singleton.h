/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: singleton.h
* Author: �����
* Version: 1.0
* Date: 2017/12/17
* Description:����ģ��

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/12/17		   1.0		  build this module
************************************************************************/
#pragma once

#include "SharedDefine.h"

//����ģ��
template<class T>
class CSingleton
{
public:
	//��ȡʵ��
	static T* Instance()
	{
		return m_pInst;
	}

	//����ʵ�������ú󣬲�����ʹ�ñ�ʵ����
	static void Release()
	{
		safe_delete(m_pInst);
	}

protected:
	//��ʵ��
	static T* m_pInst;

	//��ֹ�û�����ʵ��,�����͸�ֵ
protected:
	CSingleton() {}
	virtual ~CSingleton() {}
	DISALLOW_COPY_AND_ASSIGN(CSingleton);
};
//����ʱ���������̰߳�ȫ
template<class T>
T* CSingleton<T>::m_pInst = new T();


/* ����ģ������
class CSingletonImpl : public CSingleton<CSingletonImpl>
{
	//��ֹ�������󡢿�������ֵ
private:
	CSingletonImpl();
	~CSingletonImpl();
	DISALLOW_COPY_AND_ASSIGN(CSingletonImpl);
	friend class CSingleton<CSingletonImpl>;

	//���ܺ���
public:
	//��Ա����
private:

};
*/
