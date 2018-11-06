//
// Created by orest on 24/10/18.
//
#include <iostream>
#include <math.h>
#include "../utils/map.h"
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
	Map m = Map::fromFile(filename);
	const ulong pitch = 16;
	auto n = m.getNodesNames();
	const int chunk_size = static_cast<int>(floor(n.size() / pitch));
	for (int i = 0; i < pitch; i++) {
		dijkstra(m, *(n.begin() + i*chunk_size), *((i+1)*chunk_size < n.size() ? n.begin() + (i+1)*chunk_size : n.end() - 1));
	}
	std::cout << "Computation time Serial: " << timer.elapsed() / 1000 << "s" << std::endl;
	return 0;
}
