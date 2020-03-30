#include "ReturnState.h"

ReturnState::ReturnState(void) : FSMState("Search State") 
{
}

ReturnState::~ReturnState(void) 
{
}

float CalculateHeuristics(Node* node, Node* goal)
{
    float D = 1;
    float dx = abs(node->position.x - goal->position.x);
    float dy = abs(node->position.y - goal->position.y);
    return D * (dx + dy);
}

Node* AStar(Node* rootNode, Graph* graph, Node* goal)
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

        std::cout << currNode->id << std::endl;
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
        graph->PrintParents(true);
    }

    return NULL;
}
void ReturnState::Update(void)
{
	//std::this_thread::sleep_for(std::chrono::seconds(2));
	printf("ReturnState: Returned a resource!\n");
	mCurrentCondition = 1;

	//function ?reconstruct_path(cameFrom, current)
//	total_path : = { current }
//	while ?current ?in ?cameFrom.Keys :
//		current : = cameFrom[current]
//		total_path.append(current)
//		return ?total_path
//		function ?A_Star(start, goal)
//		// The set of nodes already evaluated
//		closedSet : = {}
//		// The set of currently discovered nodes that are not evaluated yet.
//	   // Initially, only the start node is known.
//	openSet: = { start }
//		// For each node, which node it can most efficiently be reached from.
//	  // If a node can be reached from many nodes, cameFrom will eventually contain the
//	  // most efficient previous step.
//	cameFrom: = an empty map
//		// For each node, the cost of getting from the start node to that node.
//		gScore : = map with default value of Infinity
//		// The cost of going from start to start is zero.
//		gScore[start] : = 0
//		// For each node, the total cost of getting from the start node to the goal
//	   // by passing by that node. That value is partly known, partly heuristic.
//		fScore : = map with default value of Infinity
//		// For the first node, that value is completely heuristic.
//		fScore[start] : = heuristic_cost_estimate(start, goal)
//		while ?openSet ?is not? empty
//			current : = the node in openSet having the lowest fScore[] value
//			if ?current = goal
//				return ?reconstruct_path(cameFrom, current)
//				openSet.Remove(current)
//				closedSet.Add(current)
//				for each ?neighbor ?of ?current
//					if ?neighbor ?in ?closedSet?continue // Ignore the neighbor which is already evaluated.
//					// The distance from start to a neighbor
//						tentative_gScore : = gScore[current] + dist_between(current, neighbor)
//						if ?neighbor ?not in ?openSet // Discover a new node
//							openSet.Add(neighbor)
//						else if ?tentative_gScore >= gScore[neighbor]
//							continue?;
//// This path is the best until now. Record it!
//cameFrom[neighbor] : = current
//	gScore[neighbor] : = tentative_gScore
//	fScore[neighbor] : = gScore[neighbor] + heuristic_cost_estimate(neighbor, goal)
}

void ReturnState::EnterState(void) {
	printf("ReturnState: Entered\n");
}

void ReturnState::ExitState(void) {
	printf("ReturnState: Exited\n");
}
