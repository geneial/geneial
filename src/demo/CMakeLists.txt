foreach(demo_name demo1 demo2 demo3 tsp_solver)
    add_executable(${demo_name} ${demo_name}/${demo_name}.cpp)
    target_link_libraries(${demo_name} PRIVATE geneial::geneial geneial_options)
endforeach()

add_executable(benchmark_suite benchmark_suite/benchmark_suite.cpp
    benchmark_suite/functions/Sphere.cpp benchmark_suite/functions/Ackley.cpp
    benchmark_suite/functions/F10.cpp benchmark_suite/functions/F5.cpp)
target_link_libraries(benchmark_suite PRIVATE geneial::geneial geneial_options)
