include_directories(../../src/lib)
include_directories(${CMAKE_BINARY_DIR}/generated)
#link with our genetic library
add_executable(demo3 demo3.cpp)
target_link_libraries(demo3 "${LIB_GENEIAL_SHARED_NAME}" -lpthread ${Boost_LIBRARIES})
