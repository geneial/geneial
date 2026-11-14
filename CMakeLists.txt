cmake_minimum_required(VERSION 3.21)
project(GENEIAL VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
include(CTest)
option(UNITTEST "Build unit tests (also requires BUILD_TESTING)" ${PROJECT_IS_TOP_LEVEL})
option(GENEIAL_BUILD_EXAMPLES "Build examples and benchmarks" ${PROJECT_IS_TOP_LEVEL})
add_subdirectory(src)
