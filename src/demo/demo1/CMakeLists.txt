include_directories(../../src/lib)

#TODO(bewo): this is an ugly hack to include config.h
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../generated/")


#link with our genetic library
add_executable(demo1 demo1.cpp)
target_link_libraries(demo1 "${LIB_GENEIAL_SHARED_NAME}" -lpthread ${Boost_LIBRARIES})
