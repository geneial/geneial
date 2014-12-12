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
message(STATUS "Building Source Files : ${SRCS}")


#Create a static library
add_library( ${LIB_GENEIAL_STATIC_NAME} STATIC ${SRCS} ${CONFIG_H_IN} ${HDRS} ${TMPL} )


#Also create a shared library
add_library(  ${LIB_GENEIAL_SHARED_NAME} SHARED ${SRCS} ${CONFIG_H_IN} ${HDRS} ${TMPL} )


install (TARGETS ${LIB_GENEIAL_SHARED_NAME} DESTINATION bin)
install (TARGETS ${LIB_GENEIAL_STATIC_NAME} DESTINATION bin)
install( DIRECTORY ./ 
            DESTINATION include/${LOWER_PROJECT_NAME}
            FILES_MATCHING
                PATTERN "*.h"
                PATTERN "*.hpp"
            )
