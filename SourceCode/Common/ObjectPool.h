/************************************************************************
* Copyright(c) 2017-2037 公司
* All rights reserved.
*
* license
*
* FileName: ObjectPool.h
* Author: 李辉龙
* Version: 1.0
* Date: 2018/3/2
* Description:对象池，提供重复利用的对象，减少内存开辟提高程序效率

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

	//功能函数
public:
	//获取一个可用的对象
	TObject* NewObject()
	{
		//查找空闲对象
		for (auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
		{
			if (itr->bFree)
			{
				itr->bFree = false;
				return itr->pObj;
			}
		}

		//创建新对象
		ObjItem item;
		item.pObj = new TObject();
		item.bFree = false;
		m_Objects.push_back(item);
		return item.pObj;
	}

	//回收从池子中拿出去的对象
	void Recycle(TObject* pObj)
	{
		//查找该对象
		for (auto itr = m_Objects.begin(); itr != m_Objects.end(); ++itr)
		{
			if (itr->pObj == pObj)
			{
				itr->bFree = true;
				return;
			}
		}
	}

	//清理池子
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
	//数据对象和激活状态
	struct ObjItem
	{
		TObject* pObj;
		bool bFree;
	};
	std::vector<ObjItem> m_Objects;
};

