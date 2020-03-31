#include "IdleState.h"

IdleState::IdleState(void) : FSMState("Idle State") 
{
}

IdleState::~IdleState(void) 
{
}

void IdleState::Update(void) 
{
	if (gNumResources != 0)
	{
		mCurrentCondition = 1;
	}
	else if (gNumResources == 0)
	{
		mCurrentCondition = 0;
	}
}

void IdleState::EnterState(void) 
{
	printf("IdleState: Entered\n");
}

void IdleState::ExitState(void) 
{
	printf("IdleState: Exited\n\n");
}
