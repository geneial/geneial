#TODO (bewo) make this deployment ready!
include_directories(../../src/lib)
include_directories(${CMAKE_BINARY_DIR}/generated)
#link with our genetic library
add_executable(demo1 main.cpp)
target_link_libraries(demo1 "${PROJECT_NAME}${GENEIAL_SHARED_SUFFIX}")