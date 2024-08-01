#include "Graph.h"

void Graph::addEdge(const int vertex1, const int vertex2)
{
	if (vertex1 != vertex2) {
		if (adjMatrix[vertex1][vertex2] == false && adjMatrix[vertex2][vertex1] == false)
		{
			adjMatrix[vertex1][vertex2] = true;
			adjMatrix[vertex2][vertex1] = true;
			bitset<MAX_VERTICES> maskCommon = adjMatrix[vertex1] | adjMatrix[vertex2];

			for (int i = 0; i < MAX_VERTICES; i++) {
				if (i != vertex1 && i != vertex2) {
					if (adjMatrix[vertex1][i] && adjMatrix[vertex2][i])
					{
						commonNeighbors.increase(vertex1, i);
						commonNeighbors.increase(vertex2, i);
						
					}

					else if (adjMatrix[vertex1][i])
					{
						commonNeighbors.increase(vertex2, i);
					}
					else if (adjMatrix[vertex2][i])
					{
						commonNeighbors.increase(vertex1, i);
					}
				}
			}
		}
	}
}

size_t Graph::countCommonNeighbors(int u, int v) {
	bitset<MAX_VERTICES> common = adjMatrix[u] & adjMatrix[v];
	return common.count();
}

void Graph::printCommonNeighbors(int u, int v) {
	std::cout << commonNeighbors.get(u, v);
	std::cout << endl;
}

void Graph::runsimulation(int n) {
	for (int i = 0; i < n; i++) {

		mechanism.calcProbab(commonNeighbors.sumCommNeighbors);
		int index = mechanism.selectIndex();
		int v = index;
		mechanism.calcProbab(commonNeighbors, index);
		index = mechanism.selectIndex();

		int u = index;

		addEdge(u, v);
	}
}

void Graph::load_edgelist_to_adjacency_matrix(const std::string& file_path) {
	std::ifstream infile(file_path);
	int node1, node2;

	while (infile >> node1 >> node2) {
		adjMatrix[node1][node2] = true;
		adjMatrix[node2][node1] = true; 
	}

	infile.close();
}

void Graph::initCommonNeighborsFromFile() {
	for (int i = 0; i < MAX_VERTICES; i++) {
		for (int j = 0; j < i; j++) {
			if (i != j) 
			{
				commonNeighbors.set(i, j, countCommonNeighbors(i, j));
			}
		}
	}
}

double Graph::getAverageDegree() {
	int sum = 0;
	for (int i = 0; i < MAX_VERTICES; i++) {
		sum += adjMatrix[i].count();
	}
	return  sum / MAX_VERTICES;
}

void Graph::saveEdgelistToFile(const std::string& file_path) {
	std::ofstream outfile(file_path);
	for (int i = 0; i < MAX_VERTICES; i++) {
		for (int j = 0; j < i; j++) {
			if (adjMatrix[i][j]) {
				outfile << i << " " << j << endl;
			}
		}
	}
	outfile.close();
}