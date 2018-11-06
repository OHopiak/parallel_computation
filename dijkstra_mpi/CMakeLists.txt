cmake_minimum_required(VERSION 3.12)
project(DijkstraMPI)

set(CMAKE_CXX_STANDARD 14)

find_package(MPI REQUIRED)

include_directories(${MPI_INCLUDE_PATH})

FILE(GLOB SERIAL src/serial/*)
FILE(GLOB MPI src/mpi/*)
FILE(GLOB DIJKSTRA src/dijkstra/*)
FILE(GLOB UTILS src/utils/*)

add_library(utils ${UTILS})
add_library(dijkstra ${DIJKSTRA})

add_executable(Dijkstra ${SERIAL})
target_link_libraries(Dijkstra dijkstra utils)

add_executable(DijkstraMPI ${MPI})
target_link_libraries(DijkstraMPI dijkstra utils ${MPI_LIBRARIES})

if(MPI_COMPILE_FLAGS)
	set_target_properties(DijkstraMPI PROPERTIES
			COMPILE_FLAGS "${MPI_COMPILE_FLAGS}")
endif()

if(MPI_LINK_FLAGS)
	set_target_properties(DijkstraMPI PROPERTIES
			LINK_FLAGS "${MPI_LINK_FLAGS}")
endif()
