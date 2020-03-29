#include "IdleState.h"

IdleState::IdleState(void) : FSMState("Idle State") {
}

IdleState::~IdleState(void) {
}

void IdleState::Update(void) {
	mCurrentCondition = 1;
}

void IdleState::EnterState(void) {
	printf("IdleState: Entered\n");
}

void IdleState::ExitState(void) {
	printf("IdleState: Exited\n");
}
