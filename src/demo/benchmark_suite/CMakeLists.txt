include_directories(../../src/lib)
include_directories(${CMAKE_BINARY_DIR}/generated)
#link with our genetic library
add_executable(benchmark_suite main.cpp Sphere.cpp Ackley.cpp)
target_link_libraries(benchmark_suite "${LIB_GENEIAL_SHARED_NAME}" ${Boost_LIBRARIES})
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
