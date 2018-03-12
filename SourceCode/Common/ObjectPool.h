/************************************************************************
* Copyright(c) 2017-2037 ��˾
* All rights reserved.
*
* license
*
* FileName: ObjectPool.h
* Author: �����
* Version: 1.0
* Date: 2018/3/2
* Description:����أ��ṩ�ظ����õĶ��󣬼����ڴ濪����߳���Ч��

* History:
<author>	<time>			<version>	  <desc>
lihl		2018/3/2     	   1.0		  build this module
lihl		2018/3/9     	   1.1		  ���ӳ����������������ƣ���ֹ�߱������ڴ�Ӹ߲���
************************************************************************/

#pragma once

#include<vector>

template<typename TObject>
class CObjectPool
{
public:
	CObjectPool(size_t maxnum = -1) : m_uMaxObjectNum(maxnum) {
	}
	virtual ~CObjectPool() {
		ReleaseObject();
	}

	//���ܺ���
public:
	//��ȡһ�����õĶ���
	TObject* NewObject()
	{
		//���ҿ��ж���
		for (auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
		{
			if (itr->bFree)
			{
				itr->bFree = false;
				return itr->pObj;
			}
		}

		//�����¶���
		ObjItem item;
		item.pObj = new TObject();
		item.bFree = false;
		m_Objects.push_back(item);
		return item.pObj;
	}

	//���մӳ������ó�ȥ�Ķ���
	void Recycle(TObject* pObj)
	{
		//���Ҹö���
		for (auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
		{
			if (itr->pObj == pObj)
			{
				itr->bFree = true;
				break;
			}
		}

		//ɾ������Ŀ��ж���
		if (m_Objects.size() > m_uMaxObjectNum)
		{
			PopFreeObject();
		}
	}

protected:
	void ReleaseObject()
	{
		for (auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
		{
			delete itr->pObj;
		}
	}
	void PopFreeObject()
	{
		for (auto itr = m_Objects.begin(); itr != m_Objects.end() && m_Objects.size() > m_uMaxObjectNum; )
		{
			if (itr->bFree)
			{
				delete itr->pObj;
				itr = m_Objects.erase(itr);
			}
			else
			{
				++itr;
			}
		}
	}

protected:
	//���ݶ���ͼ���״̬
	struct ObjItem
	{
		TObject* pObj;
		bool bFree;
	};
	std::vector<ObjItem> m_Objects;
	//����������
	//��new�����������ʱ����new��OK�ģ��ڻ��յ�ʱ�����ڿ��ж�����ɾ��һ���֡�
	//��ֹ�߱�����ת�͹�ʱ�ڴ�����ռ�ù���
	size_t m_uMaxObjectNum;
};

