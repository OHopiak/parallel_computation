//
// Created by orest on 26/10/18.
//
#include "floyd.h"
#include "cpu_floyd.h"
#include "gpu_floyd.h"

void floyd(const std::unique_ptr<graphFloydTopology>& data, graphAPSPAlgorithm algorithm)
{
	std::function<void(const std::unique_ptr<graphFloydTopology>&)> algorithms[] = {
			cpu_floyd,
			gpu_floyd,
	};
	algorithms[algorithm](data);
}