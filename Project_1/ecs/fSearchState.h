#ifndef _HG_fSearchState_
#define _HG_fSearchState_

#include "FSMState.h"
#include "MapInfo.h"

class SearchState : public FSMState {
public:
	SearchState(void);
	virtual ~SearchState(void);

	virtual void Update(void);

	virtual void EnterState(void);
	virtual void ExitState(void);

	std::vector<int> Dijkstra(std::vector<int> graph[], int source);
	int minDistance(std::vector<int> dist);
	int distBetween(int u, int v);
};

#endif
