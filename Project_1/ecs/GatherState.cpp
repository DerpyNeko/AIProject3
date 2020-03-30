#include "GatherState.h"
#include "bPathFollow.h"
#include "Behaviour.h"
#include "../globalStuff.h"

#include <ctime>
#include <iostream>

GatherState::GatherState(void) : FSMState("Search State") 
{
}

GatherState::~GatherState(void) 
{
}

void GatherState::Update(void)
{
	// if not at destination
	// follow path
	if (!atResource)
	{

	}

	// if at resource block
	// wait 7 seconds
	if (atResource)
	{
		currentTime = (float)std::clock();
		if ((currentTime - startTime) / (float)CLOCKS_PER_SEC > 7.0f)
		{
			resourcesHeld++;
			std::cout << "Resource gathered." << std::endl;

			// gathering done
			this->ExitState();
		}

	}
	
}

void GatherState::EnterState(void) 
{
	printf("GatherState: Entered\n");

	resourcesHeld = 0;
	startTime = (float)std::clock();
	currentTime = 0.0f;
	atResource = false;

	Path* path = new Path();

	for (int i = 0; i < dijkstraPathNodes.size(); i++)
	{
		glm::vec3 pos;
		pos.x = dijkstraPathNodes.at(i)->position.x;
		pos.y = dijkstraPathNodes.at(i)->position.y;
		pos.z = -5.0f;

		std::cout << pos.x << " " << pos.y << std::endl;

		path->pathNodes.push_back(pos);
	}

	gBehaviourManager.SetBehaviour(g_player, new PathFollowBehaviour(g_player, path));
}

void GatherState::ExitState(void) 
{
	mCurrentCondition = 2;
	printf("GatherState: Exited\n");
}
