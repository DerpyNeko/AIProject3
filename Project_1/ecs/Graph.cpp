#include "Graph.h"

#include <iostream>

Graph::Graph() {}

void Graph::CreateNode(int id, int index, Vertex position, bool bHasGoal)
{
	Node* node = new Node;
	node->id = id;
	node->index = index;
	node->position = position;
	node->hasGoal = bHasGoal;
	node->gCostSoFar = FLT_MAX;
	node->hDistance = 0;
	node->parent = NULL;
	node->visited = false;

	this->nodes.push_back(node);
}

void Graph::AddEdge(Node* parent, Node* child, float weight)
{

	if (child->id != '_')
	{
		std::pair<Node*, float> edge;

		if (child->id == 'y')
		{
			weight *= 2;
		}

		edge.first = child;
		edge.second = weight;
		parent->children.push_back(edge);

		//std::cout << "Parent: " << parent->id << " (" << parent->index << ") -> " << child->id << " (" << child->index << ") " << tempWeight << std::endl;
	}
}

void Graph::ResetGraph() {
	for (Node*& currNode : this->nodes)
	{
		currNode->visited = false;
		currNode->parent = NULL;
		currNode->gCostSoFar = FLT_MAX;
	}
}

void Graph::PrintGraph() {
	for (Node*& currNode : this->nodes)
	{
		std::cout << "Node: " << currNode->id << " -> ";
		for (std::pair <Node*, float>& child : currNode->children)
		{
			std::cout << "( " << child.first->id << ", " << child.second << ") ";
		}
		std::cout << std::endl;
	}
}

void Graph::PrintParents(bool includeCost)
{
	for (Node*& currNode : this->nodes)
	{
		if (currNode->id != '_')
		{	
			if (currNode->parent != NULL)
			{
				std::cout << "Node: " << currNode->index << " -> " << currNode->parent->index;

				for (std::pair<Node*, float> p : currNode->parent->children)
				{
					if (p.first == currNode)
					{
						std::cout << " Edge: " << p.second;
						break;
					}
				}
			}
			else
			{
				continue;
			}
			
			if (includeCost)
			{
				std::cout <<" cost so far: " << currNode->gCostSoFar;
			}
			std::cout << std::endl;
		}
	}
}