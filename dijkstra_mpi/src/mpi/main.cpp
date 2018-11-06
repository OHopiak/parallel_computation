//
// Created by orest on 24/10/18.
//
#include <iostream>
#include <mpi.h>
#include "../utils/map.h"
#include "../utils/debug.h"
#include "../utils/timer.h"
#include "../dijkstra/dijkstra.h"

int main(int argc, char* argv[])
{
	utils::Timer timer;
//	std::string filename = "../input/V10-E40";
//	std::string filename = "../input/V40-E640";
	std::string filename = "../input/V1000-E150000";
//	std::string filename = "../input/V5000-E500000";
//	std::string filename = "../input/V7500-E1125000";
//	std::string filename = "../input/V10000-E2000000";

	int mpiNodesCount, mpiNodeId;
	const int mpiRootId = 0;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &mpiNodesCount);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpiNodeId);
	std::cout << "Node #" << mpiNodeId << " initialized" << std::endl;

	Map m = Map::fromFile(filename);
	auto n = m.getNodesNames();
	const int chunk_size = static_cast<int>(n.size() / mpiNodesCount);
	dijkstra(m, *(n.begin() + mpiNodeId * chunk_size),
			 *((mpiNodeId + 1) * chunk_size < n.size() ? n.begin() + (mpiNodeId + 1) * chunk_size : n.end() - 1));
	float elapsed = 0;
	if (mpiNodeId != mpiRootId) {
		elapsed = timer.elapsed();
//		std::cout << "[" << mpiNodeId << "]" << "Sending info to master" << std::endl;
		MPI_Send(&elapsed, 1, MPI_FLOAT, mpiRootId, 0, MPI_COMM_WORLD);
	} else {
		float maxElapsed = 0.0;
//		std::cout << "[master]" << "Receiving info from nodes" << std::endl;
		for (int i = 1; i < mpiNodesCount; ++i) {
			MPI_Recv(&elapsed, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//			std::cout << "[master]" << "Node " << "[" << i << "]" << " responded" << std::endl;
			if (elapsed > maxElapsed)
				maxElapsed = elapsed;
		}
		std::cout << "Computation time MPI: " << maxElapsed / 1000 << "s" << std::endl;
	}

	MPI_Finalize();
	return 0;
}
