#include "stdafx.h"

#include <iostream>

#include "BreadthFirstSearch.h"
#include "DijkstraSearch.h"
#include "Graph.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	Graph graph;

	Node* a = new Node('a');
	graph.Add(a);
	Node* b = new Node('b');
	graph.Add(b);
	Node* c = new Node('c');
	graph.Add(c);
	Node* d = new Node('d');
	graph.Add(d);
	Node* e = new Node('e');
	graph.Add(e);
	Node* f = new Node('f');
	graph.Add(f);
	Node* g = new Node('g');
	graph.Add(g);

	graph.Connect(*a, *b, 6);
	graph.Connect(*a, *c, 2);
	graph.Connect(*b, *e, 3);
	graph.Connect(*b, *g, 3);
	graph.Connect(*c, *d, 3);
	graph.Connect(*d, *e, 1);
	graph.Connect(*e, *f, 1);
	graph.Connect(*f, *g, 1);

	cout << "Breadth First Search:" << endl;
	BreadthFirstSearch breadthFirstSearch;
	vector<Node*> path = breadthFirstSearch.Search(c, g);

	for (unsigned int uiIndex = 0; uiIndex < path.size(); uiIndex++)
	{
		cout << path.at(uiIndex)->GetName();
	}
	cout << endl;

	cout << "Dijkstra's Search:" << endl;
	DijkstraSearch dijkstraSearch;
	path = dijkstraSearch.Search(graph, c, g);

	for (unsigned int uiIndex = 0; uiIndex < path.size(); uiIndex++)
	{
		cout << path.at(uiIndex)->GetName();
	}
	cout << endl;

	char end;
	cin >> end;

	return 0;
}

