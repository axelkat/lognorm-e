#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <numeric>
#include <random>
#include <algorithm>


const int MAX_VERTICES = 1024; // Maximum number of vertices

using namespace std;


struct CommContainer {
	vector<vector<uint16_t>> commonNeighbors;
	vector<uint32_t> sumCommNeighbors;
	CommContainer(const int n) {
		init(n);
	}
	void init(const int n) {
		commonNeighbors.resize(n);
		sumCommNeighbors.resize(n);

		for (int i = 0; i < n; i++) {
			commonNeighbors[i].resize(i+1);
		}

		for (int i = 0; i < n; i++) {
			sumCommNeighbors[i] = 0;
			for(auto j = 0; j < i; j++)
			commonNeighbors[i][j] = 0; 
		}
	};

	void increase(int u, int v) {
		if (v > u) swap(u, v);
		if (commonNeighbors.size() <= u) {
			throw invalid_argument("Invalid vertex");
		}
		commonNeighbors[u][v]++;
		sumCommNeighbors[u]++;
		sumCommNeighbors[v]++;
	};

	void decrease(int u, int v) {
		if (v > u) swap(u, v);
		if (commonNeighbors.size() <= u) {
			throw invalid_argument("Invalid vertex");
		}
		commonNeighbors[u][v]--;
		sumCommNeighbors[u]--;
		sumCommNeighbors[v]--;
	};

	int get(int u, int v) {
		if (v > u) swap(u, v);
		if (commonNeighbors.size() <= u) {
			throw invalid_argument("Invalid vertex");
		}
		return commonNeighbors[u][v];
	};

	void print() {
		for (int i = 0; i < commonNeighbors.size(); i++) {
			for (int j = 0; j < i; j++) {
				cout << i << " " << j << " " << commonNeighbors[i][j] << endl;
			}
		}
	};

	void set(int u, int v, int value) {
		if (v > u) swap(u, v);
		if (commonNeighbors.size() <= u) {
			throw invalid_argument("Invalid vertex");
		}
		auto prev = commonNeighbors[u][v];
		commonNeighbors[u][v] = value;
		sumCommNeighbors[u] += value - prev;
		sumCommNeighbors[v] += value - prev;

	};


	~CommContainer() = default;
};


struct Mechanism {
	std::vector<float> probab;
	random_device rd;

	Mechanism() {
	    
		for (int i = 0; i < MAX_VERTICES; i++) {
			probab.push_back(0);
		}
	}

	~Mechanism() = default;

	void calcProbab(CommContainer& c, int u) {

		for (int i = 0; i < MAX_VERTICES; i++) {
			probab[i] = c.get(u, i);
			if (i > 0)
				probab[i] += probab[i - 1];
		}
		int sum = probab[MAX_VERTICES - 1];

		for (int i = 0; i < u; i++) {
			probab[i] /= sum;
		}
	}

	void calcProbab(std::vector<uint32_t>& c) {
		for (int i = 0; i < MAX_VERTICES; i++) {
			
			if (i > 0)
				probab[i] += probab[i - 1];
		}
		int sum = probab[MAX_VERTICES - 1];
		for (int i = 0; i < MAX_VERTICES; i++) {
			
			probab[i] /= sum;
		}
	}

	int selectIndex() {
		std::mt19937 gen(rd()); // Seed the generator
		std::uniform_real_distribution<> dis(0.0, 1.0);
		double r = dis(gen);
		auto it = lower_bound(probab.begin(), probab.end(), r);
		int index = distance(probab.begin(), it);
		return index;

	}

};

class Graph
{
private:
	vector<bitset<MAX_VERTICES>> adjMatrix;
	CommContainer commonNeighbors;
	Mechanism mechanism;

	const int n;
	

public:

	~Graph() = default;

	Graph() : n(MAX_VERTICES), commonNeighbors(MAX_VERTICES){
		adjMatrix.resize(MAX_VERTICES);
	};
	void addEdge(const int vertex1, const int vertex2);	

	size_t countCommonNeighbors(int u, int v);
	void printCommonNeighbors(int vertex1, int vertex2);
	void load_edgelist_to_adjacency_matrix(const std::string& file_path);
	void initCommonNeighborsFromFile();
	double getAverageDegree();
	void runsimulation(int n);
	void saveEdgelistToFile(const std::string& file_path);
};

