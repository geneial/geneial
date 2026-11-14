foreach(demo_name demo1 demo2 demo3 tsp_solver)
    add_executable(${demo_name} ${demo_name}/${demo_name}.cpp)
    target_link_libraries(${demo_name} PRIVATE geneial::geneial geneial_options)
endforeach()

add_executable(benchmark_suite benchmark_suite/benchmark_suite.cpp
    benchmark_suite/functions/Sphere.cpp benchmark_suite/functions/Ackley.cpp
    benchmark_suite/functions/F10.cpp benchmark_suite/functions/F5.cpp)
target_link_libraries(benchmark_suite PRIVATE geneial::geneial geneial_options)

foreach(example_name star_tour queens pixel_art)
    add_executable(${example_name} showcase/${example_name}.cpp)
    target_link_libraries(${example_name} PRIVATE geneial::geneial geneial_options)
    target_compile_definitions(${example_name} PRIVATE
        GENEIAL_ART_TARGET="${CMAKE_CURRENT_SOURCE_DIR}/showcase/assets/mona_lisa.ppm")
    if(BUILD_TESTING AND UNITTEST)
        add_test(NAME example_${example_name}
            COMMAND ${example_name} --seed 42 --generations 20
                --output "${CMAKE_CURRENT_BINARY_DIR}/${example_name}-smoke")
        set_tests_properties(example_${example_name} PROPERTIES TIMEOUT 30)
        add_test(NAME example_${example_name}_invalid_seed COMMAND ${example_name} --seed -1)
        set_tests_properties(example_${example_name}_invalid_seed PROPERTIES WILL_FAIL TRUE TIMEOUT 10)
    endif()
endforeach()
