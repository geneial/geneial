cmake_minimum_required(VERSION 2.8)

include_directories ("../lib")
find_package(Boost COMPONENTS unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})


#I like to keep test files in a separate source directory called test
file(GLOB TEST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} suites/*.cpp)

message(STATUS "Suites : ${TEST_SRC}")

#Run through each source
foreach(testSrc ${TEST_SRC})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)
		message(STATUS "Test : ${testName}")

        #Add compile target
        add_executable(${testName} ${testSrc})
		
		#link with our genetic library
		target_link_libraries(${testName} ${PROJECT_NAME})
		
        #link to Boost libraries AND your targets and dependencies
        target_link_libraries(${testName} ${Boost_LIBRARIES})

        #I like to move testing binaries into a testBin directory
        set_target_properties(${testName} PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/test_bin)

        #Finally add it to test execution - 
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName} 
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_bin 
                 COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_bin/${testName} )
endforeach(testSrc)