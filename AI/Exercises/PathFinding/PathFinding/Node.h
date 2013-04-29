#pragma once

#include <vector>

#include "Edge.h"

class Node
{
	public:
		Node(char _cName);

		~Node();

		void AddOutgoingEdge(Edge* _pEdge);

		char GetName() const;

		const std::vector<Edge*>& GetOutgoingEdges() const;

	private:
		char m_cName;

		std::vector<Edge*> m_outgoingEdges;
};
