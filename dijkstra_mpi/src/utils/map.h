//
// Created by orest on 24/10/18.
//

#ifndef DIJKSTRAMPI_MAP_H
#define DIJKSTRAMPI_MAP_H
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdexcept>
#include <sstream>
#include "debug.h"

static const std::string alphabet = "0123456789ABCDEF";

struct Map {
private:
	explicit Map(unsigned long verticesCount);

    std::vector<std::string> nodesNames;
    std::vector<int> weights;

public:
    static Map fromFile(const std::string& str);
    static Map fromFile(std::ifstream&& in);

    decltype(weights.size()) getSize() const { return weights.size(); }
    const decltype(weights) getWeights() const { return weights; }

    const decltype(nodesNames) getNodesNames() const { return nodesNames; }

    void printWeights() const;
};

#endif // DIJKSTRAMPI_MAP_H