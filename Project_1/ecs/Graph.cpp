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
	float tempWeight = weight;

	if (child->id != '_')
	{
		std::pair<Node*, float> edge;

		if (child->id == 'y')
		{
			tempWeight *= 2;
		}

		edge.first = child;
		edge.second = tempWeight;
		parent->children.push_back(edge);

		std::pair<Node*, float> reverseEdge;

		if (parent->id == 'y')
		{
			weight *= 2;
		}

		reverseEdge.first = parent;
		reverseEdge.second = weight;
		child->children.push_back(reverseEdge);
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
		std::cout << "Node: " << currNode->id << " -> ";
		if (currNode->parent != NULL)
		{
			std::cout << currNode->parent->id;
		}
		else
		{
			std::cout << "NULL";
		}
		if (includeCost)
		{
			std::cout << " cost so far: " << currNode->gCostSoFar << " hDist= " << currNode->hDistance << " f= " << currNode->gCostSoFar + currNode->hDistance;
		}
		std::cout << std::endl;
	}
}