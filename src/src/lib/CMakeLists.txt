#
# Write build specific stuff into Config.h (Version etc.)
#
set(CONFIG_H "geneial/config.h")
set(CONFIG_H_OUT "${CMAKE_BINARY_DIR}/generated/${CONFIG_H}")
set(CONFIG_H_IN  "geneial/config.h.in")

configure_file (
  "${CONFIG_H_IN}"
  "${CONFIG_H_OUT}"
)


#Make sure we can include files from this directory as root directory
#(e.g, using #include "geneial/utility/random.h")

#Read all the source files into SRC
file(GLOB_RECURSE SRCS *.cpp)
file(GLOB_RECURSE HDRS *.h)
file(GLOB_RECURSE TMPL *.hpp)


#print all the source files.
#message(STATUS "Building Source Files : ${SRCS}")
#message(STATUS "Building Source Files : ${HDRS}")
#message(STATUS "Building Source Files : ${TMPL}")

message(STATUS "Version: ${BoldCyan}${GENEIAL_VERSION_MAJOR}.${GENEIAL_VERSION_MINOR}.${GENEIAL_VERSION_PATCH}${ColourReset}")

message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_BUILD_TYPE: ${BoldCyan}${CMAKE_BUILD_TYPE}${ColourReset}")
message(STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR: ${CMAKE_CURRENT_BINARY_DIR}")

include_directories(.)
include_directories(${CMAKE_BINARY_DIR}/generated)
include_directories(${CMAKE_CURRENT_BINARY_DIR})


#
# GENEIAL STATIC:
#

#Create a static library
add_library( ${LIB_GENEIAL_STATIC_NAME} STATIC ${SRCS} ${CONFIG_H_IN} ${HDRS} ${TMPL} )

#Define GENEIAL_STATICALLY_LINKED for static lib
target_compile_definitions(${LIB_GENEIAL_STATIC_NAME} PRIVATE GENEIAL_STATICALLY_LINKED)

#geneial static will also be libgeneial
set_target_properties(${LIB_GENEIAL_STATIC_NAME} PROPERTIES OUTPUT_NAME ${LOWER_PROJECT_NAME})


#
# GENEIAL SHARED:
#

#Also create a shared library
add_library(  ${LIB_GENEIAL_SHARED_NAME} SHARED ${SRCS} ${CONFIG_H_IN} ${HDRS} ${TMPL} )

#Define GENEIAL_SHARED_LINKED for shared lib
target_compile_definitions(${LIB_GENEIAL_SHARED_NAME} PRIVATE GENEIAL_SHARED_LINKED )

#geneial static will also be libgeneial + versionize
set_target_properties(${LIB_GENEIAL_SHARED_NAME} PROPERTIES 
                                                            OUTPUT_NAME ${LOWER_PROJECT_NAME}
                                                            VERSION ${GENEIAL_VERSION_STRING}
                                                            SOVERSION ${GENEIAL_VERSION_MAJOR}
                                                            )




#
# INSTALLATION
#
#
set(LIBDEST "lib")
set(HEADERDEST "include")

install (TARGETS ${LIB_GENEIAL_SHARED_NAME} DESTINATION ${LIBDEST})
install (TARGETS ${LIB_GENEIAL_STATIC_NAME} DESTINATION ${LIBDEST})


#Warn if Debugmode is set.
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_target( DebugInfo ALL
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${COLOR} --yellow --bold "${INDENT}============"
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${COLOR} --yellow --bold "${INDENT}Note that you are about to compile the Debug Version. Compilation time and Runtime will be slower!"
    COMMAND ${CMAKE_COMMAND} -E cmake_echo_color --switch=${COLOR} --yellow --bold "${INDENT}============"
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
            
install( FILES ${CMAKE_BINARY_DIR}/generated/${CONFIG_H}
       DESTINATION ${HEADERDEST}/geneial/
        )
