cmake_minimum_required(VERSION 2.8)

configure_file (
  "Config.h.in"
  "Config.h"
)
include_directories(.)

set  (FILELIST Sources.txt )
file(READ ${FILELIST} SRC)
string(REGEX REPLACE  "#.*$"  ""  SRC  ${SRC})
string(REPLACE        "\n"    ";" SRC  ${SRC})

message(STATUS "Source Files : ${SRC}")

add_library( ${PROJECT_NAME} STATIC ${SRC} )