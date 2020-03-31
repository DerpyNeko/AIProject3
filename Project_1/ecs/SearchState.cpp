#include "SearchState.h"

SearchState::SearchState(void) : FSMState("Search State")
{
}

SearchState::~SearchState(void)
{
}

Node* SearchState::Dijkstra(Node* rootNode, Graph* graph)
{
	graph->ResetGraph();

	rootNode->visited = true;
	rootNode->gCostSoFar = 0;
	std::vector<Node*> closedList;
	std::vector<Node*> openList;
	openList.push_back(rootNode);

	while (!openList.empty())
	{
		float minCost = FLT_MAX;
		int minIndex = 0;
		Node* currNode;
		//find node with the lowest cost from root node
		for (size_t i = 0; i < openList.size(); i++)
		{
			if (openList[i]->gCostSoFar < minCost)
			{
				minCost = openList[i]->gCostSoFar;
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
			Node* n = graph->nodes[resources[gNumResources - 1].first];

			while (n->parent != NULL)
			{
				for (std::pair<Node*, float> p : n->parent->children)
				{
					if (p.first == n)
					{
						dijkstraPathNodes.push_back(p.first);
						break;
					}
				}
				n = n->parent;
			}

			dijkstraPathNodes.push_back(graph->nodes[startNode]);
			std::reverse(dijkstraPathNodes.begin(), dijkstraPathNodes.end());

			std::cout << "NODES\tCOST" << std::endl;
			for (int i = 0; i < dijkstraPathNodes.size(); i++)
			{
				std::cout << dijkstraPathNodes[i]->index;
				if (i == dijkstraPathNodes.size() - 1)
				{
					std::cout << "\t" << 0 << std::endl;
				}
				for (std::pair<Node*, float> pair : dijkstraPathNodes[i]->children)
				{
					if (i + 1 < dijkstraPathNodes.size())
					{
						if (pair.first == dijkstraPathNodes[i + 1])
						{
							std::cout << "\t" << pair.second << "\tx: " << dijkstraPathNodes[i]->position.x <<"\ty: "<< dijkstraPathNodes[i]->position.y<<std::endl;
							break;
						}
					}
				}
			}
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
					//update node when new better path is found
					child.first->gCostSoFar = weightSoFar;
					child.first->parent = currNode;
					if (!IsNodeInOpenList(openList, child.first))
					{
						openList.push_back(child.first); //add newly discovered node to open list
					}
				}
			}
		}

		//if (currNode->index == resources[0])
		//{
		//	std::cout << currNode->index << std::endl;
		//	graph->PrintParents(true);
		//}

	}

	return NULL;
}

void SearchState::Update(void)
{
	if (gNumResources == 0)
	{
		printf("SearchState: No resources found!\n");
		mCurrentCondition = 2;
	}
	else
	{
		printf("SearchState: Found %d resource(s)!\n", gNumResources);

		graph->nodes[resources[gNumResources - 1].first]->hasGoal = true;
		
		std::cout << "Starting node: " << graph->nodes[startNode]->index << " Resource: " << graph->nodes[resources[gNumResources - 1].first]->index << std::endl;
		Node* n = Dijkstra(graph->nodes[startNode], graph);

		if (n == graph->nodes[resources[gNumResources - 1].first])
		{
			graph->nodes[resources[gNumResources - 1].first]->hasGoal = false;

			startNode = graph->nodes[resources[gNumResources - 1].first]->index;

			resources[gNumResources - 1].second = false;

			gNumResources--;

			mCurrentCondition = 1;
		}
	}
}

void SearchState::EnterState(void) 
{
	printf("SearchState: Entered\n");
}

void SearchState::ExitState(void) 
{
	printf("SearchState: Exited\n\n");
}
