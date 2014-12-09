cmake_minimum_required(VERSION 2.8)


#Ensure we set the current git version
find_package(Git)
   if(GIT_FOUND)
    message("git found: ${GIT_EXECUTABLE}")
    
    # Get the current working branch
    execute_process(
      COMMAND git rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_BRANCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    message(STATUS "Git Branch is : ${GIT_BRANCH}")
    
    # Get the latest abbreviated commit hash of the working branch
    execute_process(
      COMMAND git log -1 --format=%h
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      OUTPUT_VARIABLE GIT_COMMIT_HASH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    message(STATUS "Git Revision is : ${GIT_COMMIT_HASH}")
else()
    set(GIT_COMMIT_HASH "no-rev")
    set(GIT_BRANCH "no-branch")
endif()


add_subdirectory(lib)
add_subdirectory(tests)