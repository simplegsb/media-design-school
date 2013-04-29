#pragma once

class Node;

class Edge
{
	public:
		Edge(Node* _pSource, Node* _pDestination, int _iWeight);

		Node* GetDestination() const;

		Node* GetSource() const;

		int GetWeight() const;

	private:
		Node* m_pDestination;

		Node* m_pSource;

		int m_iWeight;
};
