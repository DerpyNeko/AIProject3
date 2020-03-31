#include "GatherState.h"
#include "Behaviour.h"
#include "../globalStuff.h"

GatherState::GatherState(void) : FSMState("Search State")
{
}

GatherState::~GatherState(void)
{
}

void GatherState::Update(void)
{
	if (bHasArrived)
	{
		if (bStartGather)
		{
			std::cout << "Arrived\nNow gathering" << std::endl;

			startTime = (float)std::clock();
			bStartGather = false;
		}

		currentTime = (float)std::clock();

		if ((currentTime - startTime) / (float)CLOCKS_PER_SEC > 7.0f)
		{
			resourcesHeld++;
			std::cout << "Resource gathered" << std::endl;
			Properties* p = g_box->GetComponent<Properties>();
			p->bIsVisible = true;

			// gathering done
			mCurrentCondition = 1;
		}
	}
}

void GatherState::EnterState(void)
{
	printf("GatherState: Entered\n");

	bStartGather = true;
	resourcesHeld = 0;
	bHasArrived = false;

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
	std::cout << "Travelling to resource" << std::endl;
}

void GatherState::ExitState(void)
{
	printf("GatherState: Exited\n\n");
	dijkstraPathNodes.clear();

}
