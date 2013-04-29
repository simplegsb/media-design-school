#include "ObjectPool.h"

template<typename ObjectType>
CObjectPool<ObjectType>::CObjectPool(unsigned int _uiSize) :
	m_free(),
	m_pool()
{
	m_pool.reserve(_uiSize);
	m_pool.resize(_uiSize);
	for (unsigned int uiIndex = 0; uiIndex < _uiSize; uiIndex++)
	{
		m_free.push(&m_pool[uiIndex]);
	}
}

template<typename ObjectType>
ObjectType* CObjectPool<ObjectType>::get()
{
	if (m_free.empty())
	{
		return NULL;
	}

	ObjectType* object = m_free.front();
	m_free.pop();

	return object;
}

template<typename ObjectType>
void CObjectPool<ObjectType>::release(ObjectType* object)
{
	m_free.push(object);
}
