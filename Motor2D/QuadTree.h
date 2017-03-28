#ifndef _QUAD_TREE_H_
#define _QUAD_TREE_H_

#include <list>
#include <vector>
#include "j1Collisions.h"
#include "SDL\include\SDL.h"

#define MAX_OBJECTS 2 //Number of obects in a node before split
#define NODE_COUNT 4 //4 nodes. NW, NE, SW, SE. 

using namespace std;

class QuadTree
{
public:
	QuadTree(SDL_Rect nodeBounds);
	~QuadTree();
	void ClearTree();
	void SplitNode();
	int GetNodeIndex(Collider* col);
	void Insert(Collider* col);
	void Retrieve(list<Collider*> &potentialColliders, Collider* col);
	bool Contains(Collider* col);
	void GetNodes(vector<QuadTree*>& node);

public:
	list<Collider*> colllidersList;
	SDL_Rect nodeRect;
	QuadTree* node[4];
	int nodeIndex;
};

#endif // !_QUAD_TREE_H_

