#include "stdafx.h"

#include "Node.h"

using namespace std;

Node::Node(char _cName) :
	m_cName(_cName)
{
}

Node::~Node()
{
	for (unsigned int uiIndex = 0; uiIndex < m_outgoingEdges.size(); uiIndex++)
	{
		delete m_outgoingEdges.at(uiIndex);
	}
}

void Node::AddOutgoingEdge(Edge* _pEdge)
{
	m_outgoingEdges.push_back(_pEdge);
}

char Node::GetName() const
{
	return m_cName;
}

const vector<Edge*>& Node::GetOutgoingEdges() const
{
	return m_outgoingEdges;
}
