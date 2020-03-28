#ifndef FSMState_H
#define FSMState_H

#include <string>
#include <map>

class FSMState
{
public:
	FSMState(std::string name);
	virtual ~FSMState(void);

	void AddTransition(int condition, FSMState* state);

	virtual void Update(void) = 0;

	virtual void EnterState(void);
	virtual void ExitState(void);

	bool IsDone(void);

	FSMState* GetTransition();

protected:
	FSMState(void);
	int mCurrentCondition;

private:
	std::string mStateName;
	std::map<int, FSMState*> mTransitionMap;
};

#endif
