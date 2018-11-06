//
// Created by orest on 26/10/18.
//
#include <iostream>
#include <string>
#include <memory>
#include <ctime>
#include <algorithm>
#include <chrono>
#include <unistd.h>
#include <fstream>
#include "src/floyd.h"
#include "src/utils/timer.h"

using namespace std;
using namespace std::chrono;

unique_ptr<graphFloydTopology> readData(istream& in, int maxValue);

void printDataJson(ostream& out, const unique_ptr<graphFloydTopology>& graph, float time, int maxValue);

int main(int argc, char** argv)
{
	utils::Timer timer;
	int maxValue = MAX_DISTANCE;
	float duration;
	graphAPSPAlgorithm algorithm;

	string gpu_var = getenv("GPU") ? getenv("GPU") : "";
	bool GPU = gpu_var == "1";

//	string filename = "../input/big/V7500-E1125000"; // Computation time GPU: 131.921s
	string filename = "../input/big/V5000-E500000"; // Computation time GPU: 31.8753s
//	string filename = "../input/big/V2000-E400000"; // Computation time GPU: 2.59724s
//	string filename = "../input/big/V1000-E150000"; // Computation time GPU: 0.432362s, CPU: 169.396s
//	string filename = "../input/small/V40-E640"; // Computation time GPU: 0.133804s, CPU: 0.0137982s
//	string filename = "../input/small/V5-E10"; // Computation time

	ifstream data(filename);
	if (!data.is_open())
		return 1;
	auto graph = readData(data, maxValue);
	data.close();

	if (GPU) {
		algorithm = graphAPSPAlgorithm::GPU_FLOYD;
		cout << "Starting computing on GPU" << endl;
	} else {
		algorithm = graphAPSPAlgorithm::CPU_FLOYD;
		cout << "Starting computing on CPU" << endl;
	}

	timer.reset();
	floyd(graph, algorithm);
	duration = timer.elapsed();

	cout << "Computation time " << (GPU ? "GPU": "CPU") << ": " << duration/1000 << "s" << endl;

	/* Print graph */
//	ofstream output("../output.json");
//	printDataJson(output, graph, duration, maxValue);
//	output.flush();
//	output.close();
//	printDataJson(cout, graph, duration, maxValue);
	return 0;
}

unique_ptr<graphFloydTopology> readData(istream& in, int maxValue)
{
	unsigned int nvertex, nedges;
	int v1, v2, value;
	in >> nvertex >> nedges;

	/* Init data graph */
	unique_ptr<graphFloydTopology> data;
	data = std::make_unique<graphFloydTopology>(nvertex);
	fill_n(data->pred.get(), nvertex * nvertex, -1);
	fill_n(data->graph.get(), nvertex * nvertex, maxValue);

	/* Load data from  standard input */
	for (unsigned int i = 0; i < nedges; ++i) {
		in >> v1 >> v2 >> value;
		data->graph[v1 * nvertex + v2] = value;
		data->pred[v1 * nvertex + v2] = v1;
	}

	/* Path from vertex v to vertex v is 0 */
	for (unsigned int i = 0; i < nvertex; ++i) {
		data->graph[i * nvertex + i] = 0;
		data->pred[i * nvertex + i] = -1;
	}
	return data;
}

void printDataJson(ostream& out, const unique_ptr<graphFloydTopology>& graph, float time, int maxValue)
{
	// Lambda function for printMatrix -1 means no path
	ios::sync_with_stdio(false);
	auto printMatrix = [&out](unique_ptr<int[]>& graph, int n, int max) {
		out << "[";
		for (int i = 0; i < n; ++i) {
			out << "[";
			for (int j = 0; j < n; ++j) {
				if (max > graph[i * n + j])
					out << graph[i * n + j];
				else
					out << -1;
				if (j != n - 1) out << ",";
			}
			if (i != n - 1)
				out << "],\n";
			else
				out << "]";
		}
		out << "],\n";
	};

	out << "{\n    \"graph\":\n";
	printMatrix(graph->graph, graph->nvertex, maxValue);
	out << "    \"predecessors\": \n";
	printMatrix(graph->pred, graph->nvertex, maxValue);
	out << "    \"compute_time\": " << time << "\n}";
}
