#pragma once

#include <queue>
#include <vector>

template<typename ObjectType>
class CObjectPool
{
	public:
		CObjectPool(unsigned int _uiSize);

		ObjectType* get();

		void release(ObjectType* object);

	private:
		std::queue<ObjectType*> m_free;

		std::vector<ObjectType> m_pool;
};

#include "ObjectPool.tpp"
