include_directories(../../src/lib)

#TODO(bewo): this is an ugly hack to include config.h
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../generated/")


#link with our genetic library
add_executable(tsp_solver tsp_solver.cpp)
target_link_libraries(tsp_solver "${LIB_GENEIAL_SHARED_NAME}" -lpthread ${Boost_LIBRARIES})
