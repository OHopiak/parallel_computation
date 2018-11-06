//
// Created by orest on 26/10/18.
//

#ifndef CUDAFLOYD2_GPU_FLOYD_H
#define CUDAFLOYD2_GPU_FLOYD_H

#include "floyd.h"
#include "cuda.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#define HANDLE_ERROR(error) { \
    if (error != cudaSuccess) { \
        fprintf(stderr, "%s in %s at line %d\n", \
                cudaGetErrorString(error), __FILE__, __LINE__); \
        exit(EXIT_FAILURE); \
    } \
}

#define BLOCK_SIZE 16

void gpu_floyd(const std::unique_ptr<graphFloydTopology>& dataHost);

size_t cudaMoveMemoryToDevice(const std::unique_ptr<graphFloydTopology>& dataHost, int** graphDevice, int** predDevice);

void cudaMoveMemoryToHost(int* graphDevice, int* predDevice, const std::unique_ptr<graphFloydTopology>& dataHost,
						  size_t pitch);


#endif //CUDAFLOYD2_GPU_FLOYD_H
