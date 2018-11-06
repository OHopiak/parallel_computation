//
// Created by orest on 26/10/18.
//

#include "gpu_floyd.h"

static __global__
void gpu_floyd_kernel(const int u, size_t pitch, const int nvertex, int* const graph, int* const pred)
{
	int x = blockDim.x * blockIdx.x + threadIdx.x;
	int y = blockDim.y * blockIdx.y + threadIdx.y;

	if (y < nvertex && x < nvertex) {
		size_t indexYX = y * pitch + x;
		size_t indexUX = u * pitch + x;

		int newPath = graph[y * pitch + u] + graph[indexUX];
		int oldPath = graph[indexYX];
		if (oldPath > newPath) {
			graph[indexYX] = newPath;
			pred[indexYX] = pred[indexUX];
		}
	}
}

void gpu_floyd(const std::unique_ptr<graphFloydTopology>& dataHost)
{
	// Choose which GPU to run on, change this on a multi-GPU system.
	HANDLE_ERROR(cudaSetDevice(0));
	unsigned int nvertex = dataHost->nvertex;

	// Initialize the grid and block dimensions here
	dim3 dimGrid((nvertex - 1) / BLOCK_SIZE + 1, (nvertex - 1) / BLOCK_SIZE + 1, 1);
	dim3 dimBlock(BLOCK_SIZE, BLOCK_SIZE, 1);

	int* graphDevice, * predDevice;
	size_t pitch = cudaMoveMemoryToDevice(dataHost, &graphDevice, &predDevice);

	cudaFuncSetCacheConfig(gpu_floyd_kernel, cudaFuncCachePreferL1);
	for (int vertex = 0; vertex < nvertex; ++vertex) {
		gpu_floyd_kernel<<<dimGrid, dimBlock>>>(vertex, pitch / sizeof(size_t), nvertex, graphDevice, predDevice);
	}

	// Check for any errors launching the kernel
	HANDLE_ERROR(cudaGetLastError());
	HANDLE_ERROR(cudaDeviceSynchronize());
	cudaMoveMemoryToHost(graphDevice, predDevice, dataHost, pitch);
}

size_t cudaMoveMemoryToDevice(const std::unique_ptr<graphFloydTopology>& dataHost, int** graphDevice, int** predDevice)
{
	size_t height = dataHost->nvertex;
	size_t width = height * sizeof(int);
	size_t pitch;

	// Allocate GPU buffers for matrix of shortest paths d(G) and predecessors p(G)
	HANDLE_ERROR(cudaMallocPitch(graphDevice, &pitch, width, height));
	HANDLE_ERROR(cudaMallocPitch(predDevice, &pitch, width, height));

	// Copy input from host memory to GPU buffers and
	HANDLE_ERROR(cudaMemcpy2D(*graphDevice, pitch,
							  dataHost->graph.get(), width, width, height, cudaMemcpyHostToDevice));
	HANDLE_ERROR(cudaMemcpy2D(*predDevice, pitch,
							  dataHost->pred.get(), width, width, height, cudaMemcpyHostToDevice));

	return pitch;
}

void cudaMoveMemoryToHost(int* graphDevice, int* predDevice, const std::unique_ptr<graphFloydTopology>& dataHost,
						  size_t pitch)
{
	size_t height = dataHost->nvertex;
	size_t width = height * sizeof(int);

	HANDLE_ERROR(cudaMemcpy2D(dataHost->pred.get(), width, predDevice, pitch, width, height, cudaMemcpyDeviceToHost));
	HANDLE_ERROR(cudaMemcpy2D(dataHost->graph.get(), width, graphDevice, pitch, width, height, cudaMemcpyDeviceToHost));

	HANDLE_ERROR(cudaFree(predDevice));
	HANDLE_ERROR(cudaFree(graphDevice));
}