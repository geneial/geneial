set(CONFIG_H "geneial/config.h")
configure_file(geneial/config.h.in "${CMAKE_CURRENT_BINARY_DIR}/generated/${CONFIG_H}")
file(GLOB_RECURSE SRCS CONFIGURE_DEPENDS geneial/*.cpp)

foreach(library_kind STATIC SHARED)
    string(TOLOWER "${library_kind}" library_suffix)
    set(library_target "geneial_${library_suffix}")
    add_library(${library_target} ${library_kind} ${SRCS})
    target_include_directories(${library_target} PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/generated>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
    target_link_libraries(${library_target} PUBLIC Threads::Threads
        PRIVATE geneial_options)
    target_compile_features(${library_target} PUBLIC cxx_std_20)
    if(GENEIAL_SANITIZERS)
        target_compile_options(${library_target} PUBLIC -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${library_target} PUBLIC -fsanitize=address,undefined)
    endif()
    set_target_properties(${library_target} PROPERTIES OUTPUT_NAME geneial)
endforeach()

add_library(geneial::geneial ALIAS geneial_static)
set_target_properties(geneial_shared PROPERTIES
    VERSION ${GENEIAL_VERSION_STRING} SOVERSION ${GENEIAL_VERSION_MAJOR})

install(TARGETS geneial_static geneial_shared geneial_options EXPORT geneialTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY geneial DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/${CONFIG_H}"
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/geneial)
install(EXPORT geneialTargets NAMESPACE geneial:: DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geneial)
include(CMakePackageConfigHelpers)
configure_package_config_file("${CMAKE_CURRENT_SOURCE_DIR}/../../CMake/geneialConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/geneialConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geneial)
write_basic_package_version_file("${CMAKE_CURRENT_BINARY_DIR}/geneialConfigVersion.cmake"
    VERSION ${GENEIAL_VERSION_STRING} COMPATIBILITY SameMajorVersion)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/geneialConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/geneialConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/geneial)
