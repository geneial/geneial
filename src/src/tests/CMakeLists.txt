include_directories ("../lib")

#TODO(bewo): this is an ugly hack to include config.h
include_directories("${CMAKE_CURRENT_BINARY_DIR}/../../generated/")


#Keep test files in a separate source directory called test
file(GLOB TEST_SRC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} suites/*_test.cpp)

#message(STATUS "Suites : ${TEST_SRC}")

#make sure BOOST_MESSAGE are shown on ctest -VV
##set(testArgs "--log_level=message")

#Run through each source
foreach(testSrc ${TEST_SRC})
        #Extract the filename without an extension (NAME_WE)
        get_filename_component(testName ${testSrc} NAME_WE)
        message(STATUS "Found Unit Test: ${BoldYellow} ${testName} ${ColourReset}")

        #Add compile target
        add_executable(${testName} ${testSrc})
        
        #link with our genetic library
        target_link_libraries(${testName} "${LIB_GENEIAL_SHARED_NAME}")
        
        #link to Boost libraries AND your targets and dependencies
        target_link_libraries(${testName} ${Boost_LIBRARIES})

        #I like to move testing binaries into a testBin directory
        set_target_properties(${testName} PROPERTIES 
            RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_CURRENT_BINARY_DIR}/test_bin)
        
        
        
        #Finally add it to test execution - 
        #Notice the WORKING_DIRECTORY and COMMAND
        add_test(NAME ${testName} 
                 WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/test_bin 
                 COMMAND "${CMAKE_CURRENT_BINARY_DIR}/test_bin/${testName}" ${testArgs})
endforeach(testSrc)
