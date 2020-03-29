#ifndef _HG_MapInfo_
#define _HG_MapInfo_

#include "Entity.h"
#include "cTransform.h"
#include "cVelocity.h"

#define VERTEX 1600
extern int gNumResources;
extern int startingNode;
extern std::vector<int> graph[VERTEX];

extern std::vector<int> resources;
extern Transform* playerTransform;
extern Velocity* playerVelocity;

#endif
