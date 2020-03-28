#include "FSMSystem.h"

FSMSystem::FSMSystem(void)
	: mIsRunning(false)
	, mCurrentState(0)
{

}

FSMSystem::~FSMSystem(void)
{
	// TODO: Cleanup
}

void FSMSystem::AddState(FSMState* state)
{
	// TODO: Check if state exists in the vector before adding.
	mStateVec.push_back(state);
}

void FSMSystem::Update(void)
{
	if (!mIsRunning) return;

	if (mCurrentState == 0) {
		printf("Error: The current state is null!");
		mIsRunning = false;
		return;
	}

	mCurrentState->Update();

	if (mCurrentState->IsDone()) {
		FSMState* newState = mCurrentState->GetTransition();
		TransitionToState(newState);
	}
}

void FSMSystem::TransitionToState(FSMState* state) {
	if (state == 0) {
		printf("Error: [FSMSystem::TransitionToState] The state is null!");
		mIsRunning = false;
		return;
	}

	if (mCurrentState != 0) {
		mCurrentState->ExitState();
	}

	mCurrentState = state;
	mCurrentState->EnterState();
}

void FSMSystem::Start(void)
{
	if (mStateVec.size() == 0) {
		printf("Finite State Machine is empty! Can not start it!\n");
		return;
	}

	mIsRunning = true;
	TransitionToState(mStateVec[0]);
}

void FSMSystem::Reset(void)
{
	if (mCurrentState != 0) {
		mCurrentState->ExitState();
	}
	mCurrentState = 0;
	mIsRunning = false;
}