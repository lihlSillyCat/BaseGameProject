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
************************************************************************/

#pragma once

#include<vector>

template<typename TObject>
class CObjectPool
{
public:
	CObjectPool() {}
	~CObjectPool() {
		Clear();
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
				return;
			}
		}
	}

	//�������
	void Clear()
	{
		for (auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
		{
			if (!itr->bFree)
			{
				delete itr->pObj;
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
};

