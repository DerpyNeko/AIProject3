#ifndef _HG_fGatherState_
#define _HG_fGatherState_

#include "FSMState.h"

#include "MapInfo.h"

class GatherState : public FSMState {
public:
	GatherState(void);
	virtual ~GatherState(void);

	virtual void Update(void);

	virtual void EnterState(void);
	virtual void ExitState(void);
};

#endif
