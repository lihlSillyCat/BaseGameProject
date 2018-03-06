/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: SafeContainer.h
* Author: �����
* Version: 1.0
* Date: 2017/10/18
* Description:���߳���ʹ�õ��������̰߳�ȫ��
����ʹ�� C++ STL ���

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/18		   1.0		  build this module
************************************************************************/
#pragma once

#include <windows.h>
#include <deque>
#include <list>
#include <vector>

//��ȫ���У��Ƚ��ȳ���
//������������������ģʽ�������߳�push�������߳�pop
template<typename _Type> class safe_queue 
{
	typedef _Type value_type;

public:
	safe_queue()
	{
		::InitializeSRWLock(&m_SRWLock);
		m_pIn = &m_container[0];
		m_pOut = &m_container[1];
	}

	~safe_queue()
	{
		::AcquireSRWLockExclusive(&m_SRWLock);
		m_container[0].clear();
		m_container[1].clear();
		::ReleaseSRWLockExclusive(&m_SRWLock);
	}

public:
	//��ĩβ����һ��Ԫ��
	void push(value_type value)
	{
		::AcquireSRWLockExclusive(&m_SRWLock);
		if (m_pIn->size() < m_pIn->max_size())
		{
			m_pIn->push_back(value);
		}
		else 
		{
			swap_container();
			if (m_pIn->size() < m_pIn->max_size())
			{
				m_pIn->push_back(value);
			}
		}
		::ReleaseSRWLockExclusive(&m_SRWLock);
	}

	//������һ��Ԫ��
	//Ϊ��ʱ����false
	bool pop(value_type &value)
	{
		bool res = false;
		::AcquireSRWLockExclusive(&m_SRWLock);
		if (!m_pOut->empty())
		{
			value = m_pOut->front();
			m_pOut->pop_front();
			res = true;
		}
		else if (!m_pIn->empty())
		{
			swap_container();
			value = m_pOut->front();
			m_pOut->pop_front();
			res = true;
		}
		else
		{
			res = false;
		}
		::ReleaseSRWLockExclusive(&m_SRWLock);
		return res;
	}

	//�Ƿ�Ϊ��
	bool empty()
	{
		bool res = false;
		::AcquireSRWLockShared(&m_SRWLock);
		res = (m_pIn->empty() && m_pOut->empty());
		::ReleaseSRWLockShared(&m_SRWLock);
		return res;
	}

	//�������
	void clear()
	{
		::AcquireSRWLockExclusive(&m_SRWLock);
		m_container[0].clear();
		m_container[1].clear();
		::ReleaseSRWLockExclusive(&m_SRWLock);
	}

	//�������
	std::size_t max_size()
	{
		std::size_t max;
		::AcquireSRWLockShared(&m_SRWLock);
		max = m_pIn->max_size();
		::ReleaseSRWLockShared(&m_SRWLock);
		return max;
	}
	
protected:
	//��������
	void swap_container()
	{
		auto inout = m_pIn;
		m_pIn = m_pOut;
		m_pOut = inout;
	}

private:
	//�ڲ�ʹ�ý������У����Ч�ʲ��Ҽ���������ļ���
	//���ý��ڲ�����ԭ��¶���û�
	std::deque<value_type> m_container[2];
	std::deque<value_type> *m_pIn;
	std::deque<value_type> *m_pOut;
	//��д������֤�̰߳�ȫ(�߳�ͬ���ж�д��Ч����Խϸ�)
	SRWLOCK m_SRWLock;	

	//��ֹ�����͸�ֵ
private:
	safe_queue(const safe_queue&);
	void operator=(const safe_queue&);
};

//
////�̰߳�ȫ��vector
////��δд��
//template<typename _Type> class safe_vector
//{
//	typedef _Type value_type;
//
//public:
//	safe_vector()
//	{
//		::InitializeSRWLock(&m_SRWLock);
//	}
//
//	~safe_vector()
//	{
//		::AcquireSRWLockExclusive(&m_SRWLock);
//		m_data.clear();
//		::ReleaseSRWLockExclusive(&m_SRWLock);
//	}
//
//
//
//private:
//	std::vector<value_type> m_data;
//	//��д������֤�̰߳�ȫ(�߳�ͬ���ж�д��Ч����Խϸ�)
//	SRWLOCK m_SRWLock;
//
//	//��ֹ�����͸�ֵ
//private:
//	safe_vector(const safe_vector&);
//	void safe_vector =(const safe_vector&);
//};
