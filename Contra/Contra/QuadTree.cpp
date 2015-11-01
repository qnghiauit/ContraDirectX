#include "Quadtree.h"

#if MEMORY_LEAK_DEBUG == 1
#include <vld.h>
#endif

#pragma region Node

Node::Node()
{
	FOR(NUM_OF_SUBNODE)
		_subNode[i] = 0;
}


Node::Node(RECT* range) : _range(range)
{
	FOR(NUM_OF_SUBNODE)
		_subNode[i] = 0;
}


Node::Node(RECT* range, vector<Object*>& objectList) : _range(range)
{
	this->assign(objectList);
}


Node::~Node()
{
	SAFE_RELEASE(_range);
}


// =====================================
// Name: Node:release()
// Desc: Delete all sub node from "this".
// De-recurred release method
// =====================================
void Node::release()
{
	vector<Node*> nodeList;
	nodeList.push_back(this);

	while (!nodeList.empty()) {
		if (nodeList.back()->_subNode[0] != 0) {

			Node* checkingNode = nodeList.back();
			nodeList.pop_back();

			FOR(NUM_OF_SUBNODE)
				nodeList.push_back(checkingNode->_subNode[i]);
			}
		else {
			SAFE_RELEASE(nodeList.back());
			nodeList.pop_back();
		}
	}
}


// =====================================
// Name: Node:assign()
// Desc: Check and select the object(s) bounded with this node by it rect to store in the node.
// If there are many objects are stored and the node is big enough, it'll be self-split.
// De-recurred assign method
// =====================================
void Node::assign(vector<Object*>& objectList)
{
	vector<Node*> nodeList;
	nodeList.push_back(this);

	while (true) {

	AGAIN:
		FOR(objectList.size())
			if (Default::checkIfBounded(nodeList.back()->_range, &objectList[i]->getRect()))
				nodeList.back()->_nodeObjects.push_back(objectList[i]);

		if (nodeList.back()->isSplitable()) {

		PARENT_SPLIT:
			FOR(NUM_OF_SUBNODE)
				if (nodeList.back()->_subNode[i] == 0) {
				nodeList.back()->_subNode[i] = new Node(caculateSubNodeRange(i, *nodeList.back()->_range));
				objectList = nodeList.back()->_nodeObjects;
				nodeList.push_back(nodeList.back()->_subNode[i]);
				goto AGAIN;
				}

			nodeList.back()->_nodeObjects.clear();
		}

		nodeList.pop_back();

		if (nodeList.size() == 0)
			break;
		else
			goto PARENT_SPLIT;
	}

}


// =====================================
// Name: Node:getObjects()
// Desc: Check and return the Objects it's (and it's sub nodes) storing
// if the current node is bounded with the cameraRange.
// De-recurred get method
// =====================================
vector<Object*> Node::getObjects(const RECT& cameraRange)
{
	vector<Node*> nodeList;
	nodeList.push_back(this);
	vector<Object*> result;

	while (true) {

		Node* checkingNode = nodeList.back();

		// If the node range is bounded with current camera view port.
		if (Default::checkIfBounded(&cameraRange, checkingNode->_range)) {

			// If it have the sub nodes.
			if (checkingNode->_subNode[0] != 0)
				#pragma region Replace the node by it subnodes
			{
				nodeList.pop_back();
				FORJ(NUM_OF_SUBNODE)
					nodeList.push_back(checkingNode->_subNode[j]);
			}
#pragma endregion
			else
				#pragma region Add storing objects into result list
			{
				FOR(checkingNode->_nodeObjects.size()) {
					vector<Object*>* checkingList = &checkingNode->_nodeObjects;

					if (checkingList->at(i) != 0)
						result.push_back(checkingList->at(i));
					else {
						checkingList->erase(checkingList->begin() + i);
						i--;
					}
				}
			}
				nodeList.pop_back();
#pragma endregion

		}
		else nodeList.pop_back();

		if (!nodeList.empty())
			continue;

		return result;
	}
}


bool Node::isSplitable()
{
	LONG node_W = _range->right - _range->left;
	LONG node_H = _range->top - _range->bottom;

	bool tooManyObject = _nodeObjects.size() > NODE_MAX_OBJECT;
	bool sizeBigEnough = MIN(node_W, node_H) > (NUM_OF_NODE_PER_ROW * MAX(RESOLUTION_WIDTH, RESOLUTION_HEIGHT));

	return (tooManyObject && sizeBigEnough);
}


RECT* Node::caculateSubNodeRange(int index, const RECT& baseRect)
{
	RECT* result = new RECT();
	LONG node_W = (baseRect.right - baseRect.left) / NUM_OF_NODE_PER_ROW;
	LONG node_H = (baseRect.top - baseRect.bottom) / NUM_OF_NODE_PER_ROW;

	result->left = baseRect.left + node_W * (index % NUM_OF_NODE_PER_ROW);
	result->right = result->left + node_W;
	result->bottom = baseRect.bottom + node_H * (index / NUM_OF_NODE_PER_ROW);
	result->top = result->bottom + node_H;

	return result;
}

#pragma endregion

#pragma region Tree

QuadTree::QuadTree()
{
}


QuadTree::QuadTree(RECT* range)
{
	_root = new Node(range);
}


QuadTree::~QuadTree()
{
	_root->release();
	SAFE_RELEASE(_root);
}


void QuadTree::assign(vector<Object*>& objectList)
{
	_assignedObjects = objectList;
	_root->assign(objectList);
}


vector<Object*> QuadTree::getObjectInRange(RECT range)
{
	vector<Object*> result = _root->getObjects(range);
	// removeDuplicate(result);

	return result;
}


void QuadTree::removeDuplicate(vector<Object*>& objectList)
{
	FORJ(objectList.size() - 1)
		FOR_AB(j + 1, objectList.size())
		if (objectList[i] == objectList[j]) {
			objectList.erase(objectList.begin() + i);
			i--;
		}
}


void QuadTree::update()
{
	FOR(this->_assignedObjects.size())
		this->_assignedObjects[i]->update();
}


void QuadTree::draw(RECT range)
{
	vector<Object*> drawObjects = QuadTree::getObjectInRange(range);

	FOR(drawObjects.size())
		if (Default::checkIfBounded(&range, &(drawObjects[i]->Object::getRect())))
			drawObjects[i]->draw();

#if DEBUG == DEBUG_QUADTREE_DRAW_OBJECT
		OutputDebugString(Default::tostr(drawObjects.size()).c_str());
		OutputDebugString(L"\n");
#endif
}
#pragma endregion