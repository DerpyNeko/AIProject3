#ifndef IdleState_H
#define IdleState_H

#include "FSMState.h"

#include "MapInfo.h"

class IdleState : public FSMState {
public:
	IdleState(void);
	virtual ~IdleState(void);

	virtual void Update(void);

	virtual void EnterState(void);
	virtual void ExitState(void);

};

#endif
