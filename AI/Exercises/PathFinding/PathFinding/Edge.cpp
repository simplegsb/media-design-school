#include "stdafx.h"
#include "Edge.h"

Edge::Edge(Node* _pSource, Node* _pDestination, int _iWeight) :
	m_pDestination(_pDestination), m_pSource(_pSource), m_iWeight(_iWeight)
{
}

Node* Edge::GetDestination() const
{
	return m_pDestination;
}

Node* Edge::GetSource() const
{
	return m_pSource;
}

int Edge::GetWeight() const
{
	return m_iWeight;
}
