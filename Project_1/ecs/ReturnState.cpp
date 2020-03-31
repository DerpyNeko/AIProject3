#include "ReturnState.h"
#include "bPathFollow.h"
#include "../globalStuff.h"

#include <ctime>
#include <algorithm>

ReturnState::ReturnState(void) : FSMState("Return State")
{
}

ReturnState::~ReturnState(void)
{
}

float ReturnState::CalculateHeuristics(Node* node, Node* goal)
{
	float D = 1;
	float D2 = 2;
	float dx = abs(node->position.x - goal->position.x);
	float dy = abs(node->position.y - goal->position.y);
	return D * (dx + dy) + (D2 - 2 * D) * std::min(dx, dy);
}

Node* ReturnState::AStar(Node* rootNode, Graph* graph, Node* goal)
{
	graph->ResetGraph();

	rootNode->gCostSoFar = 0;
	rootNode->hDistance = CalculateHeuristics(rootNode, goal);

	std::vector<Node*> closedList;
	std::vector<Node*> openList;
	openList.push_back(rootNode);

	while (!openList.empty())
	{
		float minCost = FLT_MAX;
		int minIndex = 0;
		Node* currNode;
		//find node with the lowest cost from root node and heuristic distance from the goal node
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i]->gCostSoFar + openList[i]->hDistance < minCost)
			{
				minCost = openList[i]->gCostSoFar + openList[i]->hDistance;
				minIndex = i;
			}
		}

		//remove current node from open list and add to closed list
		currNode = openList[minIndex];
		for (auto iter = openList.begin(); iter != openList.end(); ++iter)
		{
			if (*iter == currNode)
			{
				openList.erase(iter);
				break;
			}
		}
		closedList.push_back(currNode);

		currNode->visited = true;
		if (currNode->hasGoal)
		{
			Node* n = graph->nodes[homeNode];

			while (n->parent != NULL)
			{
				for (std::pair<Node*, float> p : n->parent->children)
				{
					if (p.first == n)
					{
						aStarPathNodes.push_back(p.first);
						break;
					}
				}
				n = n->parent;
			}

			aStarPathNodes.push_back(graph->nodes[startNode]);
			std::reverse(aStarPathNodes.begin(), aStarPathNodes.end());

			std::cout << "NODES\tCOST\tA.COST\tHDIST\tFVALUE" << std::endl;
			for (int i = 0; i < aStarPathNodes.size(); i++)
			{
				std::cout << aStarPathNodes[i]->index;
				if (i == aStarPathNodes.size() - 1)
				{
					std::cout << "\t" << 0 << "\t" << aStarPathNodes[i]->gCostSoFar <<"\t" << aStarPathNodes[i]->hDistance << "\t" << aStarPathNodes[i]->hDistance + aStarPathNodes[i]->gCostSoFar << std::endl;
				}
				for (std::pair<Node*, float> pair : aStarPathNodes[i]->children)
				{
					if (i + 1 < aStarPathNodes.size())
					{
						if (pair.first == aStarPathNodes[i + 1])
						{
							std::cout << "\t" << pair.second << "\t" << pair.first->gCostSoFar << "\t" << pair.first->hDistance << "\t" << pair.first->hDistance + pair.first->gCostSoFar << std::endl;
							break;
						}
					}
				}
			}
			std::cout << std::endl;
			return currNode;
		}

		//Go through every child node node 
		for (std::pair <Node*, float> child : currNode->children)
		{
			if (child.first->visited == false)
			{
				float weightSoFar = currNode->gCostSoFar + child.second;
				if (weightSoFar < child.first->gCostSoFar)
				{
					child.first->gCostSoFar = weightSoFar;
					child.first->parent = currNode;
					if (!IsNodeInOpenList(openList, child.first))
					{
						child.first->hDistance = CalculateHeuristics(child.first, goal);
						openList.push_back(child.first);
					}
				}
			}
		}
		//graph->PrintParents(true);
	}

	return NULL;
}

void ReturnState::Update(void)
{
	if (bHasArrived)
	{	
		if (bUnloadResource)
		{
			std::cout << "Arrived at base." << std::endl;
			startTime = (float)std::clock();
			bUnloadResource = false;
		}

		currentTime = (float)std::clock();

		if ((currentTime - startTime) / (float)CLOCKS_PER_SEC > 2.0f)
		{
			std::cout << "Resource unloaded at base." << std::endl;
			Properties* p = g_box->GetComponent<Properties>();
			p->bIsVisible = false;
			startNode = homeNode;
			graph->nodes[homeNode]->hasGoal = false;
			mCurrentCondition = 1;
		}
	}
}

void ReturnState::EnterState(void)
{
	printf("ReturnState: Entered\n");

	graph->nodes[homeNode]->hasGoal = true;
	std::cout << "Starting node: " << graph->nodes[startNode]->index << " Home: " << graph->nodes[homeNode]->index << std::endl;
	Node* n = AStar(graph->nodes[startNode], graph, graph->nodes[homeNode]);

	if (n == graph->nodes[homeNode])
	{
		Path* path = new Path();

		for (int i = 0; i < aStarPathNodes.size(); i++)
		{
			glm::vec3 pos;
			pos.x = aStarPathNodes.at(i)->position.x;
			pos.y = aStarPathNodes.at(i)->position.y;
			pos.z = -5.0f;

			path->pathNodes.push_back(pos);
		}

		gBehaviourManager.SetBehaviour(g_player, new PathFollowBehaviour(g_player, path));

		std::cout << "Returning to base." << std::endl;
	}

	bHasArrived = false;
	bUnloadResource = true;
}

void ReturnState::ExitState(void)
{
	printf("ReturnState: Exited\n\n");
}
