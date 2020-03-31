#ifndef ReturnState_H
#define ReturnState_H

#include "FSMState.h"

#include "MapInfo.h"

class ReturnState : public FSMState {
public:
	ReturnState(void);
	virtual ~ReturnState(void);

	virtual void Update(void);

	virtual void EnterState(void);
	virtual void ExitState(void);

	Node* AStar(Node* rootNode, Graph* graph, Node* goal);
	float CalculateHeuristics(Node* node, Node* goal);

	std::vector<Node*> aStarPathNodes;

	int resourcesHeld;

	bool bUnloadResource = false;

	float startTime;
	float currentTime;

};

#endif
