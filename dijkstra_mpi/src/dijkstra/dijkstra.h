//
// Created by orest on 25/10/18.
//

#ifndef DIJKSTRAMPI_DIJKSTRA_H
#define DIJKSTRAMPI_DIJKSTRA_H

#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <stdexcept>
#include <limits>
#include "../utils/map.h"

void dijkstra(const Map& m, const std::string& initialNodeName, const std::string& goalNodeName);

#endif //DIJKSTRAMPI_MPI_DIJKSTRA_H
