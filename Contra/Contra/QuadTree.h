#ifndef __QUADTREE_H__
#define __QUADTREE_H__

#include <vector>

#include "Object.h"
#include "GameDefaultConstant.h"

#pragma region Node

class Node
{
public:
	Node();
	Node(RECT* range);
	Node(RECT* range, vector<Object*>& objectList);
	~Node();

	void assign(vector<Object*>& objectList);
	vector<Object*> getObjects(const RECT& cameraRange);
	void release();

private:
	bool isSplitable();
	RECT* caculateSubNodeRange(int index, const RECT& baseRect);

private:
	vector<Object*> _nodeObjects;

	// Sub node indexes signed as
	// 2 3
	// 0 1
	Node* _subNode[4];

	RECT* _range;
};

#pragma endregion

#pragma region Tree

class QuadTree
{
public:
	QuadTree();
	QuadTree(RECT* range);
	~QuadTree();

	void assign(vector<Object*>& objectList);
	void update();
	void draw(RECT range);

private:
	void removeDuplicate(vector<Object*>& objectList);
	vector<Object*> getObjectInRange(RECT range);

private:
	Node* _root;
	vector<Object*> _assignedObjects;
};

#pragma endregion

#endif
