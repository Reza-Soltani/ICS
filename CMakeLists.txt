cmake_minimum_required(VERSION 3.12)
project(MaximumCommunityInfluence)

set(CMAKE_CXX_STANDARD 17)

add_executable(MaximumCommunityInfluence main.cpp BiGraph.cpp BiGraph.h InfluenceCalculator.cpp InfluenceCalculator.h)