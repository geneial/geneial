# MakeLists files in this project can
# refer to the root source directory of the project as ${GENETICLIBRARY_SOURCE_DIR} and
# to the root binary directory of the project as ${GENETICLIBRARY_BIN_DIR}.
cmake_minimum_required(VERSION 2.8)
#Setup CMake to run tests
enable_testing()

project (GENEIAL)
string(TOLOWER ${PROJECT_NAME} LOWER_PROJECT_NAME)

set (GENEIAL_SHARED_SUFFIX "_shared")
set (LIB_GENEIAL_SHARED_NAME "${LOWER_PROJECT_NAME}${GENEIAL_SHARED_SUFFIX}")

set (GENEIAL_STATIC_SUFFIX "_static")
set (LIB_GENEIAL_STATIC_NAME "${LOWER_PROJECT_NAME}${GENEIAL_STATIC_SUFFIX}")


set (GENEIAL_VERSION_MAJOR 0)
set (GENEIAL_VERSION_MINOR 1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -Wextra -Wno-unused-parameter")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_subdirectory(src)
add_subdirectory(demo)


# add a target to generate API documentation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
    add_custom_target(
        doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
else()
    message(STATUS "Doxygen not found! Will render no documentation!")
endif(DOXYGEN_FOUND)
