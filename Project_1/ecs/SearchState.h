#ifndef SearchState_H
#define SearchState_H

#include "FSMState.h"

#include "MapInfo.h"

class SearchState : public FSMState {
public:
	SearchState(void);
	virtual ~SearchState(void);

	virtual void Update(void);

	virtual void EnterState(void);
	virtual void ExitState(void);

	Node* Dijkstra(Node* rootNode, Graph* graph);
	bool IsNodeInOpenList(std::vector<Node*> openList, Node* child);
};

#endif
