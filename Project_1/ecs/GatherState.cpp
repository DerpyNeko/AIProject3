#include "GatherState.h"
#include "Behaviour.h"
#include "../globalStuff.h"

GatherState::GatherState(void) : FSMState("Search State"), currentTime(0), resourcesHeld(0), startTime(0)
{
}

GatherState::~GatherState(void)
{
}

void GatherState::Update(void)
{
	if (bHasCollected)
	{
		std::cout << "Arrived" << std::endl;

		currentTime = (float)std::clock();
		if ((currentTime - startTime) / (float)CLOCKS_PER_SEC > 7.0f)
		{
			resourcesHeld++;
			//resources.erase(resources.begin());
			std::cout << "Resource gathered" << std::endl;

			// gathering done
			mCurrentCondition = 1;
		}
	}
}

void GatherState::EnterState(void) 
{
	printf("GatherState: Entered\n");

	resourcesHeld = 0;
	startTime = (float)std::clock();
	currentTime = 0.0f;

	Path* path = new Path();

	for (int i = 0; i < dijkstraPathNodes.size(); i++)
	{
		glm::vec3 pos;
		pos.x = dijkstraPathNodes.at(i)->position.x;
		pos.y = dijkstraPathNodes.at(i)->position.y;
		pos.z = -5.0f;

		path->pathNodes.push_back(pos);
	}

	gBehaviourManager.SetBehaviour(g_player, new PathFollowBehaviour(g_player, path));
}

void GatherState::ExitState(void) 
{
	printf("GatherState: Exited\n");
	std::cout << "Resources: " << resources.size() << std::endl;
}
