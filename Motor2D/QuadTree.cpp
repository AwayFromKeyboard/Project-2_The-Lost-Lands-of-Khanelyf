#include "QuadTree.h"

//TODO 2

QuadTree::QuadTree(SDL_Rect nodeBounds)
{
	node[0] = node[1] = node[2] = node[3] = nullptr;
	nodeIndex = 0;
	nodeRect = nodeBounds;
}

QuadTree::~QuadTree()
{

}

void QuadTree::ClearTree()
{
	colllidersList.clear();

	for (int i = 0; i < NODE_COUNT; i++) {
		if (node[0] != nullptr) {
			node[i]->ClearTree();
			RELEASE(node[i]);
		}
	}
}

void QuadTree::SplitNode()
{
	int halfWidth = nodeRect.w / 2;
	int halfHeight = nodeRect.h / 2;
	int x = nodeRect.x;
	int y = nodeRect.y;

	node[0] = new QuadTree(SDL_Rect{ x, y, halfWidth, halfHeight }); //NW Node
	node[1] = new QuadTree(SDL_Rect{ x + halfWidth, y, halfWidth, halfHeight }); //NE Node
	node[2] = new QuadTree(SDL_Rect{ x, y + halfHeight, halfWidth, halfHeight }); //SW Node
	node[3] = new QuadTree(SDL_Rect{ x + halfWidth, y + halfHeight, halfWidth, halfHeight }); //SE Node

	node[0]->nodeIndex = 0;
	node[1]->nodeIndex = 1;
	node[2]->nodeIndex = 2;
	node[3]->nodeIndex = 3;
}

int QuadTree::GetNodeIndex(Collider* col)
{
	int index = -1;

	for (int i = 0; i < NODE_COUNT; i++) {
		if (node[i] != nullptr) {
			if (node[i]->Contains(col)) {
				index = node[i]->nodeIndex;
			}
		}
	}

	return index;
}

void QuadTree::Insert(Collider* col)
{

	if (node[0] != nullptr) {
		int index = GetNodeIndex(col);

		if (index != -1) {
			node[index]->Insert(col);

			return;
		}
	}

	colllidersList.push_back(col);

	if (colllidersList.size() > MAX_OBJECTS) {
		if (node[0] == nullptr) {
			SplitNode();
		}

		int i = 0;
		while (i < colllidersList.size()) {
			list<Collider*>::iterator it = colllidersList.begin();
			for (int j = 0; j < i; j++) {
				it++;
			}

			int index = GetNodeIndex(*it);
			if (index != -1) {
				node[index]->Insert(*it);
				colllidersList.remove(*it);
			}
			else {
				i++;
			}
		}
	}
}

void QuadTree::Retrieve(list<Collider*> &possibleColliders, Collider* col)
{

	int index = GetNodeIndex(col);
	if (index != -1 && node[0] != nullptr) {
		node[index]->Retrieve(possibleColliders, col);
	}
	else {
		for (list<Collider*>::iterator it = colllidersList.begin(); it != colllidersList.end(); it++) {
			//Skip checking with himself
			if (col != *it) {
				possibleColliders.push_back(*it);
			}
		}
	}
}

bool QuadTree::Contains(Collider * col)
{
	return (col->rect.x >= nodeRect.x && col->rect.x + col->rect.w <= nodeRect.x + nodeRect.w &&
		col->rect.y >= nodeRect.y && col->rect.y + col->rect.h <= nodeRect.y + nodeRect.h);
}

void QuadTree::GetNodes(vector<QuadTree*>& nodeList)
{
	if (node[0] != nullptr) {
		for (int i = 0; i < NODE_COUNT; i++) {
			node[i]->GetNodes(nodeList);
		}
	}
	else {
		nodeList.push_back(this);
	}

}
