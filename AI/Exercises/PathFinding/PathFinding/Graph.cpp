#include "stdafx.h"

#include "Graph.h"

using namespace std;

Graph::~Graph()
{
	for (unsigned int uiIndex = 0; uiIndex < m_nodes.size(); uiIndex++)
	{
		delete m_nodes.at(uiIndex);
	}
}

void Graph::Add(Node* _pNode)
{
	m_nodes.push_back(_pNode);
}

void Graph::Connect(Node& _rA, Node& _rB, int _iWeight)
{
	_rA.AddOutgoingEdge(new Edge(&_rA, &_rB, _iWeight));
	_rB.AddOutgoingEdge(new Edge(&_rB, &_rA, _iWeight));
}

Node* Graph::Get(char name) const
{
	for (unsigned int uiIndex = 0; uiIndex < m_nodes.size(); uiIndex++)
	{
		if (m_nodes.at(uiIndex)->GetName() == name)
		{
			return m_nodes.at(uiIndex);
		}
	}

	return NULL;
}

const vector<Node*>& Graph::GetAll() const
{
	return m_nodes;
}

unsigned int Graph::Size() const
{
	return m_nodes.size();
}
