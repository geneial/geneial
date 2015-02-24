include_directories(../../src/lib)
include_directories(${CMAKE_BINARY_DIR}/generated)
#link with our genetic library
add_executable(demo1 main.cpp)
target_link_libraries(demo1 "${LIB_GENEIAL_SHARED_NAME}" ${Boost_LIBRARIES})
message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
