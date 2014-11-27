# MakeLists files in this project can
# refer to the root source directory of the project as ${GENETICLIBRARY_SOURCE_DIR} and
# to the root binary directory of the project as ${GENETICLIBRARY_BIN_DIR}.
cmake_minimum_required(VERSION 2.8)
#Setup CMake to run tests
enable_testing()



project (OASysGL)

set (OASysGL_SHARED_SUFFIX "_shared")
set (OASysGL_STATIC_SUFFIX "_static")

set (OASysGL_VERSION_MAJOR 0)
set (OASysGL_VERSION_MINOR 1)


set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_subdirectory(src)
add_subdirectory(demo)