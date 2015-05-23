cmake_minimum_required(VERSION 2.8)

#Write build specific stuff into Config.h (Version etc.)

set(CONFIG_H "config.h")
set(CONFIG_H_IN "geneial/config.h.in")

configure_file (
  "${CONFIG_H_IN}"
  "${CMAKE_BINARY_DIR}/generated/geneial/${CONFIG_H}"
)

include_directories(${CMAKE_BINARY_DIR}/generated)


#Make sure we can include files from this directory as root directory
#(e.g, using #include "geneial/utility/random.h")
include_directories(.)

#Read all the source files into SRC
file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h)
file(GLOB_RECURSE TMPL *.hpp)


#print all the source files.
#message(STATUS "Building Source Files : ${SRCS}")
#message(STATUS "Building Source Files : ${HDRS}")
#message(STATUS "Building Source Files : ${TMPL}")
message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_BUILD_TYPE: ${BoldCyan}${CMAKE_BUILD_TYPE}${ColourReset}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")


#Create a static library
add_library( ${LIB_GENEIAL_STATIC_NAME} STATIC ${SRCS} ${CONFIG_H_IN} ${HDRS} ${TMPL} )

#Also create a shared library
add_library(  ${LIB_GENEIAL_SHARED_NAME} SHARED ${SRCS} ${CONFIG_H_IN} ${HDRS} ${TMPL} )
cotire(${LIB_GENEIAL_STATIC_NAME})
cotire(${LIB_GENEIAL_SHARED_NAME})

set(LIBDEST "lib")
set(HEADERDEST "include")

install (TARGETS ${LIB_GENEIAL_SHARED_NAME} DESTINATION ${LIBDEST})
install (TARGETS ${LIB_GENEIAL_STATIC_NAME} DESTINATION ${LIBDEST})

#SET(CMAKE_COLOR_MAKEFILE ON)
#SET(CMAKE_VERBOSE_MAKEFILE ON)
#set_target_properties(mylib PROPERTIES DEBUG_POSTFIX "debug")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_target( DebugInfo ALL
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${COLOR} --yellow --bold "${INDENT}"
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${COLOR} --yellow --bold "${INDENT}Note that you are about to compile the Debug Version. Compilation time and Runtime will be slower!"
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${COLOR} --yellow --bold "${INDENT}"
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${COLOR} --yellow --bold ""
    COMMENT "Debug Notice")
    add_dependencies( ${LIB_GENEIAL_SHARED_NAME} DebugInfo )
    add_dependencies( ${LIB_GENEIAL_STATIC_NAME} DebugInfo )
endif()

install( DIRECTORY ./ 
            DESTINATION include
            FILES_MATCHING
                PATTERN "*.h"
                PATTERN "*.hpp"
            )
            
#also install the generated config file
install( FILES ${CMAKE_BINARY_DIR}/generated/geneial/${CONFIG_H}
            DESTINATION ${HEADERDEST}/geneial/
            )
