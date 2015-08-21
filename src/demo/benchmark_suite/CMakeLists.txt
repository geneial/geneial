include_directories(../../src/lib)
include_directories(${CMAKE_BINARY_DIR}/generated)
#link with our genetic library
add_executable(benchmark_suite benchmark_suite.cpp functions/Sphere.cpp functions/Ackley.cpp functions/F10.cpp functions/F5.cpp)
target_link_libraries(benchmark_suite "${LIB_GENEIAL_SHARED_NAME}" pthread ${Boost_LIBRARIES})
