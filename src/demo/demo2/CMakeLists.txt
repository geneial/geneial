include_directories(../../src/lib)
include_directories(${CMAKE_BINARY_DIR}/generated)
#link with our genetic library
add_executable(demo2 demo2.cpp)
target_link_libraries(demo2 "${LIB_GENEIAL_SHARED_NAME}" -lpthread ${Boost_LIBRARIES})
