#include "SearchState.h"
#include <iostream>

SearchState::SearchState(void) : FSMState("Search State") 
{
}

SearchState::~SearchState(void) 
{
}

bool SearchState::IsNodeInOpenList(std::vector<Node*> openList, Node* child)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i] == child)
			return true;

	}
	return false;
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

		if (currNode->index == resources[0])
		{
			std::cout << currNode->index << std::endl;
			graph->PrintParents(true);
		}

	}

	std::cout << "NODES:" << std::endl;
	Node* n = graph->nodes[resources[0]];

	while (n->parent != NULL)
	{
		std::cout << n->index << " : ";
		for (std::pair<Node*, float> p : n->parent->children)
		{
			if (p.first == n)
			{
				std::cout << p.second << std::endl;
				dijkstraPathNodes.push_back(p.first);
				break;
			}
			
		}

		n = n->parent;
	}

	dijkstraPathNodes.push_back(graph->nodes[startNode]);
	std::reverse(dijkstraPathNodes.begin(), dijkstraPathNodes.end());

	std::cout << "REVERSED" << std::endl;
		std::cout << "Positions: " << std::endl;
	for (Node* n : dijkstraPathNodes)
	{
		std::cout << n->index << std::endl;
		std::cout << n->position.x << " " << n->position.y << std::endl;


	}

	return NULL;
}

void SearchState::Update(void) 
{
	if (gNumResources == 0) {
		printf("SearchState: No resources found!\n");
		mCurrentCondition = 2;
	}
	else {
		printf("SearchState: Found %d resources!\n", gNumResources);

		// graph->nodes[resources[0]]->hasGoal = true;
		std::cout << "STarting node: " << graph->nodes[startNode]->index << " Resource: " << graph->nodes[resources[0]]->index << std::endl;
		Node* n = Dijkstra(graph->nodes[startNode], graph);

		if (n == graph->nodes[resources[0]])
		{
			resources.erase(resources.begin());
		}

		mCurrentCondition = 1;
	}
}

void SearchState::EnterState(void) {
	printf("SearchState: Entered\n");
}

void SearchState::ExitState(void) {
	printf("SearchState: Exited\n");
}
