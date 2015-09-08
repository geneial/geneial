#
#
#   ____            _          _    _     
#  / ___| ___ _ __ (_) __ _   / \  | |    
# | |  _ / _ \ '_ \| |/ _` | / _ \ | |    
# | |_| |  __/ | | | | (_| |/ ___ \| |___ 
#  \____|\___|_| |_|_|\__,_/_/   \_\_____|
#                                        
#
#

cmake_minimum_required(VERSION 3.1 FATAL_ERROR)



# 1.) Project Meta Information
#
project ("GENEIAL" C CXX)

set( PROJECT_FULLNAME "GeneiAL" )
set( PROJECT_DESCRIPTION "Genetic Algorithm Library" )

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)

#
# 2.) Includes and Helper
#

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")
include(vars)
include(use_cxx11)
include(version)
include(colors)

#
# 3.) Compile (ccmake) options
#

option( GPERF "Compile for gperf" OFF )
option( UNITTEST "Compile Unit Test" ON )
option( CLANG_SANATIZE "Compile with Clang Sanatizers enabled" ON )
option( USE_NINJA "Support Ninja Buildsystem" ON )


#
# Build Debug Version if nothing was specified.
#
if( NOT CMAKE_BUILD_TYPE )
   set( CMAKE_BUILD_TYPE Debug )
endif()


#
# Compile with Unit Test Support?
#
if (NOT DEFINED UNITTEST OR ${UNITTEST})
    enable_testing()
endif()

#
# Boost Support 
#
find_package(Boost COMPONENTS unit_test_framework system REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})

#
# Compiler switches
#
use_cxx11()

# Common switches:
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -pedantic-errors -Wextra -Werror -Wno-unused-parameter -Wno-unused-variable")

# Compiler specific: 
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 3.3)
        message(FATAL_ERROR " clang version must be at least 3.3!")
    endif()
    
    if(GPERF)
        message(FATAL_ERROR " Cannot use GPERF with clang!")
    endif()
    
    if(CLANG_SANATIZE)
        message(" Building with Clang Sanatizers!")
        set (CLANG_SANATIZEEXTRA "address") #memory error detector. Slowdown: 2x#    
        set (CLANG_SANATIZEEXTRA "${CLANG_SANATIZERS},thread") 
        set (CLANG_SANATIZERS "${CLANG_SANATIZERS},undefined") #undefined behavior checks,  small runtime cost 
        #set (CLANG_SANATIZERS "${CLANG_SANATIZERS},bool") 
        set (CLANG_SANATIZEEXTRA "${CLANG_SANATIZERS},float-divide-by-zero")
        #set (CLANG_SANATIZERS "${CLANG_SANATIZERS},unreachable")
        set (CLANG_SANATIZEEXTRA "${CLANG_SANATIZERS},float-cast-overflow")
        #set (CLANG_SANATIZERS "${CLANG_SANATIZERS},return")
        #set (CLANG_SANATIZEEXTRA "${CLANG_SANATIZERS},memory")
        set (CLANG_SANATIZEEXTRA "${CLANG_SANATIZERS},bounds")
        set (CLANG_SANATIZEEXTRA "${CLANG_SANATIZERS},integer")
        set (CLANG_SANATIZEEXTRA "-fsanitize=${CLANG_SANATIZERS} -Wthread-safety")
    endif()
#    
    #shiny output even in ninja
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=1000 -fcolor-diagnostics")
#    
    #enable sanatizers    
#    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${CLANG_SANATIZEEXTRA}")
#endif()
    

    set(CMAKE_CXX_RELEASE_FLAGS "${CMAKE_CXX_RELEASE_FLAGS} -O3 -fomit-frame-pointer -msse -msse2 -funroll-loops")


    if(USE_NINJA)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ferror-limit=1000 -fcolor-diagnostics")
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    if(GPERF)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -gperf")
    endif()
    
    #Produce additional debugging information for gdb:
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb")
    
else()

    message(WARNING "Unsupported CXX Compiler!")

endif()

# Print final flags:

message("-- CXX Flags: ${CMAKE_CXX_FLAGS}")
message("-- CXX Debug Flags: ${CMAKE_CXX_DEBUG_FLAGS}")
message("-- CXX Release Flags: ${CMAKE_CXX_RELEASE_FLAGS}")
message("-- Build type: ${CMAKE_BUILD_TYPE}")


#
# 4.) Subdirectories
#

add_subdirectory(src)
add_subdirectory(demo)


#
# 5.) Doxygen Documentation Target (make doc)
#
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile @ONLY)
    add_custom_target(
        doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/doc/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc
        COMMENT "Generating API documentation with Doxygen" VERBATIM
    )
else()
    message(WARNING "Doxygen not found! Will render no documentation!")
endif(DOXYGEN_FOUND)

