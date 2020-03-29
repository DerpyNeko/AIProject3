#include "fGatherState.h"

GatherState::GatherState(void)
	: FSMState("Search State") {
}

GatherState::~GatherState(void) {
}

void GatherState::Update(void) {
	printf("GatherState: Gathered a resource!\n");
	gNumResources--;
	mCurrentCondition = 1;
}

void GatherState::EnterState(void) {
	// printf("GatherState: Entered\n");
}

void GatherState::ExitState(void) {
	// printf("GatherState: Exited\n");
}
