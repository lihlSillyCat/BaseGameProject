/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: SafeContainer.h
* Author: 李辉龙
* Version: 1.0
* Date: 2017/10/18
* Description:多线程中使用的容器。线程安全。
容器使用 C++ STL 风格

* History:
<author>	<time>			<version>	  <desc>
lihl		2017/10/18		   1.0		  build this module
************************************************************************/
#pragma once

#include <windows.h>
#include <deque>
#include <list>
#include <vector>

//安全队列（先进先出）
//适用于生产者消费者模式，数据线程push，工作线程pop
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
	//在末尾插入一个元素
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

	//弹出第一个元素
	//为空时返回false
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

	//是否为空
	bool empty()
	{
		bool res = false;
		::AcquireSRWLockShared(&m_SRWLock);
		res = (m_pIn->empty() && m_pOut->empty());
		::ReleaseSRWLockShared(&m_SRWLock);
		return res;
	}

	//清除容器
	void clear()
	{
		::AcquireSRWLockExclusive(&m_SRWLock);
		m_container[0].clear();
		m_container[1].clear();
		::ReleaseSRWLockExclusive(&m_SRWLock);
	}

	//最大容量
	std::size_t max_size()
	{
		std::size_t max;
		::AcquireSRWLockShared(&m_SRWLock);
		max = m_pIn->max_size();
		::ReleaseSRWLockShared(&m_SRWLock);
		return max;
	}
	
protected:
	//交换容器
	void swap_container()
	{
		auto inout = m_pIn;
		m_pIn = m_pOut;
		m_pOut = inout;
	}

private:
	//内部使用交换队列，提高效率并且减少锁互斥的几率
	//不用将内部工作原理暴露给用户
	std::deque<value_type> m_container[2];
	std::deque<value_type> *m_pIn;
	std::deque<value_type> *m_pOut;
	//读写锁，保证线程安全(线程同步中读写锁效率相对较高)
	SRWLOCK m_SRWLock;	

	//禁止拷贝和赋值
private:
	safe_queue(const safe_queue&);
	void operator=(const safe_queue&);
};

//
////线程安全的vector
////暂未写完
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
//	//读写锁，保证线程安全(线程同步中读写锁效率相对较高)
//	SRWLOCK m_SRWLock;
//
//	//禁止拷贝和赋值
//private:
//	safe_vector(const safe_vector&);
//	void safe_vector =(const safe_vector&);
//};
