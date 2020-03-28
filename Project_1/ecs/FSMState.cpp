#include "FSMState.h"

FSMState::FSMState(void)
	: mCurrentCondition(0) {
}

FSMState::FSMState(std::string name)
	: mStateName(name)
	, mCurrentCondition(0) {
}

FSMState::~FSMState(void) {
	// TODO: Cleanup
}

void FSMState::AddTransition(int condition, FSMState* state) {
	std::map<int, FSMState*>::iterator it = mTransitionMap.find(condition);
	if (it != mTransitionMap.end()) {
		printf("FSMState::AddTransition: State already has this condition!\n");
		return;
	}
	mTransitionMap[condition] = state;
}

void FSMState::Update(void) {
	mCurrentCondition = 1;
}

void FSMState::EnterState(void) {
	printf("Entering %s state...\n", mStateName.c_str());
}

void FSMState::ExitState(void) {
	printf("Exiting %s state...\n", mStateName.c_str());
}

bool FSMState::IsDone(void) {
	return mCurrentCondition != 0;
}

FSMState* FSMState::GetTransition() {
	if (!IsDone()) return 0;
	return mTransitionMap[mCurrentCondition];
}