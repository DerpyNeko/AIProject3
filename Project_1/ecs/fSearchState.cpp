#include "fSearchState.h"

SearchState::SearchState(void): FSMState("Search State") {
}

SearchState::~SearchState(void) {
}

int SearchState::minDistance(std::vector<int> dist)
{
	int min = INT_MAX;
	int min_index = 0;

	for (int v = 0; v < VERTEX; v++)
	{
		if (dist[v] <= min)
		{
			min = dist[v];
			min_index = v;
		}
	}

	return min_index;
}

int SearchState::distBetween(int u, int v)
{
	return u - v;
}

std::vector<int> SearchState::Dijkstra(std::vector<int> graph[], int source)
{
	std::vector<int> dist(VERTEX);
	std::vector<int> previous(VERTEX);

	for (int v : *graph)
	{
		dist[v] = INT_MAX;
		previous[v] = -1;
	}

	dist[source] = 0;

	std::vector<int> Q = *graph;
	while (Q.size() > 0)
	{
		int u = minDistance(dist);
		Q.erase(Q.begin() + u);

		for (int v : graph[u])
		{
			int alt = dist[u] + distBetween(u, v);
			if (alt < dist[v])
			{
				dist[v] = alt;
				previous[v] = u;
			}
		}
	}

	return previous;
}
//	function ​Dijkstra(Graph, source) :
//		for each ​vertex v ​in ​Graph : // Initialization
//			dist[v] : = infinity // initial distance from source to vertex v is set to infinite
//			previous[v] : = undefined // Previous node in optimal path from source
//		dist[source] : = 0 // Distance from source to source
//		Q : = the set of all nodes in Graph // all nodes in the graph are unoptimized - thus are in Q
//		while ​Q ​is not empty : // main loop
//			u : = node in Q with smallest dist[]
//			remove u from Q
//			for ​each neighbor v of u : // where v has not yet been removed from Q​.
//				alt: = dist[u] + dist_between(u, v)
//				if ​alt < dist[v] // Relax (u,v)
//					dist[v] : = alt
//					previous[v] : = u
//	return ​previous[]

void SearchState::Update(void) {
	if (gNumResources == 0) {
		printf("SearchState: No resources found!\n");
		mCurrentCondition = 2;
	}
	else {
		printf("SearchState: Found %d resources!\n", gNumResources);
		Dijkstra(graph, startingNode);
		mCurrentCondition = 1;
	}
}

void SearchState::EnterState(void) {
	printf("SearchState: Entered\n");
}

void SearchState::ExitState(void) {
	printf("SearchState: Exited\n");
}
