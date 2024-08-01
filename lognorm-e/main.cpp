#include <iostream>
#include "Graph.h"


int main()
{
	Graph g;
	g.load_edgelist_to_adjacency_matrix("C:\\Users\\rdode\\Downloads\\BA.txt");
	g.initCommonNeighborsFromFile();
	std::cout<< "Graph loaded\n!" <<std::endl;

	for(int i = 0; i < 3000; i++)
	{
		g.runsimulation(100);
		std::cout<<g.getAverageDegree()<<std::endl;
	}

	g.saveEdgelistToFile("C:\\Users\\rdode\\Downloads\\BA2.txt");
	return 0;
}