cmake_minimum_required(VERSION 3.21)
project(GENEIAL VERSION 0.3.0 LANGUAGES CXX)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
include(version)
include(GNUInstallDirs)
include(CTest)
set(CMAKE_CXX_EXTENSIONS OFF)

option(UNITTEST "Build unit tests (also requires BUILD_TESTING)" ON)
option(GENEIAL_HEADER_ONLY_TESTS "Use header-only Boost.Test (avoids compiler ABI mismatches)" OFF)
option(GENEIAL_BUILD_EXAMPLES "Build examples and benchmarks" ON)
option(GENEIAL_SANITIZERS "Enable address and undefined behavior sanitizers" OFF)
option(GENEIAL_EMBED_GIT_METADATA "Embed local branch and commit in generated config.h" OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

find_package(Threads REQUIRED)
include(CheckStandardLibrary)
if(BUILD_TESTING AND UNITTEST)
    if(GENEIAL_HEADER_ONLY_TESTS)
        find_package(Boost 1.74 REQUIRED)
    else()
        find_package(Boost 1.74 REQUIRED COMPONENTS unit_test_framework)
    endif()
endif()

add_library(geneial_options INTERFACE)
target_compile_features(geneial_options INTERFACE cxx_std_20)
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(geneial_options INTERFACE -Wall -Wextra -Wno-unused-parameter -Wno-unused-variable)
elseif(GENEIAL_SANITIZERS)
    message(FATAL_ERROR "GENEIAL_SANITIZERS requires Clang or GCC")
endif()

add_subdirectory(src)
if(GENEIAL_BUILD_EXAMPLES)
    add_subdirectory(demo)
endif()

find_package(Doxygen QUIET)
if(DOXYGEN_FOUND)
    configure_file(Doxyfile.in doc/Doxyfile @ONLY)
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        VERBATIM)
endif()
