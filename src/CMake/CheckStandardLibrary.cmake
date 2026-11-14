include(CheckCXXSourceCompiles)

function(geneial_check_standard_library)
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_REQUIRED_LIBRARIES Threads::Threads)
    check_cxx_source_compiles("
        #include <span>
        #include <thread>
        int main() {
            int values[] = {1};
            std::span<int> view{values};
            std::jthread worker([] {});
            return view.front() - 1;
        }" GENEIAL_HAS_CXX20_LIBRARY)
    if(NOT GENEIAL_HAS_CXX20_LIBRARY)
        message(FATAL_ERROR "GeneiAL requires C++20 std::span and std::jthread. Update the standard library as well as the compiler (libstdc++ 12+, libc++ 20+, or a current Xcode toolchain).")
    endif()
endfunction()

geneial_check_standard_library()
