#ifndef _HG_MapInfo_
#define _HG_MapInfo_

#include "Entity.h"
#include "cTransform.h"
#include "cVelocity.h"
#include "Graph.h"

#include <iostream>
#include <ctime>

extern int gNumResources;
extern int homeNode;
extern int startNode;

extern Graph* graph;
extern std::vector<Node*> dijkstraPathNodes;

extern std::vector<int> resources;
extern Transform* playerTransform;
extern Velocity* playerVelocity;

inline bool IsNodeInOpenList(std::vector<Node*> openList, Node* child)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i] == child)
			return true;

	}
	return false;
}

#endif
