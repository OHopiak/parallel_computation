//
// Created by orest on 24/10/18.
//

#ifndef DIJKSTRAMPI_DEBUG_H
#define DIJKSTRAMPI_DEBUG_H

#include <iostream>

#define LOG(x) do { \
        std::cout << x << std::endl; \
} while(0)

#define LOGf(x) do { \
//    std::cout << __func__ << ": " << #x << " = '" << (x) << "'" << std::endl; \
} while(0)

#define LOGv(v) do { \
    std::cout << #v << " = [ "; \
    for(auto&& iiii: v) \
        std::cout << iiii << " "; \
    std::cout << "]" << std::endl; \
} while(0)

#endif // DIJKSTRAMPI_DEBUG_H
