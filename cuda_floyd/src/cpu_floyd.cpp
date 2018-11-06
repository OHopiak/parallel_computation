//
// Created by orest on 26/10/18.
//

#include "floyd.h"

void cpu_floyd(const std::unique_ptr<graphFloydTopology>& data)
{
	int newPath = 0;
	int n = data->nvertex;

	for (int u = 0; u < n; ++u) {
		for (int v1 = 0; v1 < n; ++v1) {
			for (int v2 = 0; v2 < n; ++v2) {
				newPath = data->graph[v1 * n + u] + data->graph[u * n + v2];
				if (data->graph[v1 * n + v2] > newPath) {
					data->graph[v1 * n + v2] = newPath;
					data->pred[v1 * n + v2] = data->pred[u * n + v2];
				}
			}
		}
	}
}