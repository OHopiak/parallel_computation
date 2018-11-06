//
// Created by orest on 24/10/18.
//
#include "map.h"

Map::Map(unsigned long verticesCount) : weights(verticesCount)
{
	weights.resize(verticesCount * verticesCount);
}

Map Map::fromFile(const std::string& str)
{
	return fromFile(std::ifstream(str.c_str()));
}

Map Map::fromFile(std::ifstream&& in)
{
	std::string header;
	ulong verticesCount, nedges;
	int v1, v2, value;
	in >> verticesCount >> nedges;

	if (verticesCount <= 0) throw std::runtime_error("Wrong map file format");

	Map m(verticesCount);
	std::fill(m.weights.begin(), m.weights.end(), -1);

	for (unsigned int i = 0; i < nedges; ++i) {
		in >> v1 >> v2 >> value;
		m.weights[v1 * verticesCount + v2] = value;
	}

/*
	for (auto i = 0; i < verticesCount; ++i) {
		for (auto j = 0; j < verticesCount; ++j) {
			in >> m.weights[i * verticesCount + j];
		}
	}
*/


	for (auto i = 0; i < verticesCount; ++i) {
		std::string nodeName;
		auto index = i;
		if (index == 0) nodeName = "0";
		while (index != 0) {
			nodeName += alphabet[index % alphabet.size()];
			index /= alphabet.size();
		}
//		std::cout << "Adding node: " << nodeName << std::endl;
		m.nodesNames.emplace_back(nodeName);
	}
	return m;

}

void Map::printWeights() const
{
	for (auto i = 0u; i < getSize(); ++i) {
		for (auto j = 0u; j < getSize(); ++j)
			if (weights[i * getSize() + j] != -1)
				std::cout << std::setw(4) << weights[i * getSize() + j] << " ";
			else
				std::cout << std::setw(4) << "-" << " ";

		std::cout << std::endl;
	}
}
