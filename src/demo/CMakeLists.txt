foreach(demo_name demo1 demo2 demo3 tsp_solver)
    add_executable(${demo_name} ${demo_name}/${demo_name}.cpp)
    target_link_libraries(${demo_name} PRIVATE geneial::geneial geneial_options)
endforeach()

set(GENEIAL_SITE_DIR "${CMAKE_BINARY_DIR}/site")
add_custom_target(website
    COMMAND ${CMAKE_COMMAND} -E make_directory "${GENEIAL_SITE_DIR}/examples"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/../../docs/site" "${GENEIAL_SITE_DIR}"
    COMMAND $<TARGET_FILE:star_tour> --seed 42 --generations 2000 --output "${GENEIAL_SITE_DIR}/examples/star_tour"
    COMMAND $<TARGET_FILE:queens> --seed 42 --generations 3000 --output "${GENEIAL_SITE_DIR}/examples/queens"
    COMMAND $<TARGET_FILE:pixel_art> --seed 42 --generations 5000 --output "${GENEIAL_SITE_DIR}/examples/pixel_art"
    COMMAND $<TARGET_FILE:voronoi_art> --seed 42 --threads 4 --shapes 24 --max-shapes 256 --generations 6000 --output "${GENEIAL_SITE_DIR}/examples/voronoi_art"
    COMMAND $<TARGET_FILE:triangle_art> --seed 42 --threads 4 --shapes 96 --generations 12000 --output "${GENEIAL_SITE_DIR}/examples/triangle_art"
    COMMAND $<TARGET_FILE:maze> --seed 42 --size 18 --generations 1000 --output "${GENEIAL_SITE_DIR}/examples/maze"
    DEPENDS star_tour queens pixel_art voronoi_art triangle_art maze
    COMMENT "Evolving six experiments and building the static gallery"
    VERBATIM)

add_executable(benchmark_suite benchmark_suite/benchmark_suite.cpp
    benchmark_suite/functions/Sphere.cpp benchmark_suite/functions/Ackley.cpp
    benchmark_suite/functions/F10.cpp benchmark_suite/functions/F5.cpp)
target_link_libraries(benchmark_suite PRIVATE geneial::geneial geneial_options)

foreach(example_name star_tour queens pixel_art voronoi_art triangle_art maze)
    add_executable(${example_name} showcase/${example_name}.cpp)
    target_link_libraries(${example_name} PRIVATE geneial::geneial geneial_options)
    target_compile_definitions(${example_name} PRIVATE
        GENEIAL_ART_TARGET="${CMAKE_CURRENT_SOURCE_DIR}/showcase/assets/mona_lisa.ppm")
    if(BUILD_TESTING AND UNITTEST)
        set(showcase_smoke_options)
        if(example_name MATCHES "^(voronoi_art|triangle_art)$")
            set(showcase_smoke_options --population 20 --shapes 8 --resolution 32 --threads 2)
        endif()
        add_test(NAME example_${example_name}
            COMMAND ${example_name} --seed 42 --generations 20 ${showcase_smoke_options}
                --output "${CMAKE_CURRENT_BINARY_DIR}/${example_name}-smoke")
        set_tests_properties(example_${example_name} PROPERTIES TIMEOUT 30)
        if(GENEIAL_COVERAGE)
            set_tests_properties(example_${example_name} PROPERTIES TIMEOUT 120)
        endif()
        add_test(NAME example_${example_name}_invalid_seed COMMAND ${example_name} --seed -1)
        set_tests_properties(example_${example_name}_invalid_seed PROPERTIES WILL_FAIL TRUE TIMEOUT 10)
    endif()
endforeach()
