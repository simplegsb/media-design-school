#pragma once

#include "Node.h"

class Graph
{
	public:
		~Graph();

		void Add(Node* _pNode);

		void Connect(Node& _rA, Node& _rB, int _iWeight);

		Node* Get(char name) const;

		const std::vector<Node*>& GetAll() const;

		unsigned int Size() const;

	private:
		std::vector<Node*> m_nodes;
};
