//
// Created by orest on 26/10/18.
//

#ifndef CUDAFLOYD2_FLOYD_H
#define CUDAFLOYD2_FLOYD_H

#include <memory>
#include <functional>

#define MAX_DISTANCE (1 << 30) - 1

typedef enum {
	CPU_FLOYD = 0,
	GPU_FLOYD = 1,
} graphAPSPAlgorithm;

/* Default structure for graph */
struct graphFloydTopology {
	unsigned int nvertex; // number of vertices in graph
	std::unique_ptr<int[]> pred; // predecessors matrix
	std::unique_ptr<int[]> graph; // graph matrix

	/* Constructor for init fields */
	explicit graphFloydTopology(unsigned int nvertex): nvertex(nvertex) {
		int size_squared = nvertex * nvertex;
		pred = std::unique_ptr<int[]>(new int[size_squared]);
		graph = std::unique_ptr<int[]>(new int[size_squared]);
	}
};

void floyd(const std::unique_ptr<graphFloydTopology>& data, graphAPSPAlgorithm algorithm);

#endif //CUDAFLOYD2_FLOYD_H
