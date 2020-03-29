#pragma once

#include <list>
#include <vector>

struct Vertex
{
    Vertex() : x(0), y(0), z(0) {}

    Vertex(float iX, float iY, float iZ) :
        x(iX),
        y(iY),
        z(iZ)
    {
    }

    float x, y, z;
};

struct Node
{
    char id;
    int index;
    bool visited;
    bool hasGoal;
    float gCostSoFar;
    float hDistance;
    Vertex position;     //position in the game world. used for calculating heuristic distances
    struct Node* parent; //parent Node that we can follow back to the root node.
    std::vector<std::pair< Node*, float>> children; //Edges pointing to neighbouring graph <childNode, edgeWeight>
};

class Graph
{
public:
    Graph();
    void CreateNode(int id, int index, Vertex position, bool bHasGoal = false);
    void AddEdge(Node* parent, Node* child, float weight);
    void ResetGraph();
    void PrintGraph();
    void PrintParents(bool includeCost = false);

    std::vector<Node*> nodes;
};
