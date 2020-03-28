#include "ReturnState.h"
#include <thread>
#include <chrono>

ReturnState::ReturnState(void)
	: FSMState("Search State") {
}

ReturnState::~ReturnState(void) {
}

void ReturnState::Update(void)
{
	std::this_thread::sleep_for(std::chrono::seconds(2));
	printf("ReturnState: Returned a resource!\n");
	mCurrentCondition = 1;
}

void ReturnState::EnterState(void) {
	// printf("ReturnState: Entered\n");
}

void ReturnState::ExitState(void) {
	// printf("ReturnState: Exited\n");
}
