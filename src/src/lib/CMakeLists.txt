cmake_minimum_required(VERSION 2.8)

#Write build specific stuff into Config.h (Version etc.)
configure_file (
  "Config.h.in"
  "Config.h"
)
#Make sure we can include files from this directory as root directory
#(e.g, using #include "utility/random.h")
include_directories(.)

#Read all the source files into SRC
file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h)
file(GLOB_RECURSE TMPL *.hpp)


#print all the source files.
message(STATUS "Building Source Files : ${SRC}")

#Create a static library
add_library( ${LIB_OASysGL_STATIC_NAME} STATIC ${SRCS} ${HDRS} ${TMPL} )


#Also create a shared library
add_library(  ${LIB_OASysGL_SHARED_NAME} SHARED ${SRCS} ${HDRS} ${TMPL} )


install (TARGETS ${LIB_OASysGL_SHARED_NAME} DESTINATION bin)
install (TARGETS ${LIB_OASysGL_STATIC_NAME} DESTINATION bin)
install( DIRECTORY ./ 
            DESTINATION include/${PROJECT_NAME}
            FILES_MATCHING PATTERN "*.h" )
