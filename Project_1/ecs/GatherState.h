#ifndef GatherState_H
#define GatherState_H

#include "FSMState.h"

#include "MapInfo.h"

class GatherState : public FSMState {
public:
	GatherState(void);
	virtual ~GatherState(void);

	virtual void Update(void);

	virtual void EnterState(void);
	virtual void ExitState(void);

	int resourcesHeld;
	bool atResource;

	float startTime;
	float currentTime;
};

#endif
