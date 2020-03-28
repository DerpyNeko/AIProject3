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

};

#endif
