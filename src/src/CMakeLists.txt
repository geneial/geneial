set(GIT_BRANCH "unknown")
set(GIT_COMMIT_HASH "unknown")
if(GENEIAL_EMBED_GIT_METADATA)
    find_package(Git QUIET)
    if(GIT_FOUND)
        execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            OUTPUT_VARIABLE branch OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET RESULT_VARIABLE branch_result)
        execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
            OUTPUT_VARIABLE revision OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET RESULT_VARIABLE revision_result)
        if(branch_result EQUAL 0 AND revision_result EQUAL 0)
            set(GIT_BRANCH "${branch}")
            set(GIT_COMMIT_HASH "${revision}")
        endif()
    endif()
endif()

add_subdirectory(lib)
if(BUILD_TESTING AND UNITTEST)
    add_subdirectory(tests)
endif()
