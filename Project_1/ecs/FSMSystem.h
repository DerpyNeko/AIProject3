#ifndef FSMSystem_H
#define FSMSystem_H

#include <vector>
#include <string>

#include "FSMState.h"

class FSMSystem
{
public:
	FSMSystem(void);
	virtual ~FSMSystem(void);

	void AddState(FSMState* state);
	void TransitionToState(FSMState* state);

	void Update(void);

	void Start(void);
	void Reset(void);

private:
	std::vector<FSMState*> mStateVec;
	bool mIsRunning;

	FSMState* mCurrentState;
};

#endif
