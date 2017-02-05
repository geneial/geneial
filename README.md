libGeneiAL - A C++ Genetic Algorithm Library 
--------------------------------------------

[![Build Status](https://travis-ci.org/geneial/geneial.svg?branch=master)](https://travis-ci.org/geneial/geneial)
[![DUB](https://img.shields.io/dub/l/vibe-d.svg)]()
[![Docker Automated buil](https://img.shields.io/docker/automated/jrottenberg/ffmpeg.svg)](https://hub.docker.com/r/geneial/geneial/)

##1. Building the library

Compability: We require a C++ 11 compiler such as clang-3.5+ or gcc-4.7+.
Furthermore, we require the Boost Library >= 1.55 to be installed.

###1.1 The easy way
There is a script that takes care of the building and installation process.
Simply run

    ./autobuilder.sh 

###1.2 The manual way [Debug Mode]

To build the genetic library use the following commands

    mkdir -p build/
    cd build/
    cmake ../src
    make

Make sure there is no `Cmakecache.txt` in the main directory, otherwise cmake does not work in out of source folder.

You'll find the static Libary in the `build/src/lib` library.

####1.2.1 Make parameters
You may add `make -j8` whereas `8` denotes the number of CPU cores, to run make multithreaded, which speeds up things.

You may further add `make VERBOSE=1` to see whats going on.

####1.2.2 Debug vs. Release

By default the Debug version is build which is build without compiler optimization. While the Debug version is convenient for debugging and testing, it is **approximately 10-14 times slower**. In order to build the Release version (Optimization level 3) execute 

    cmake -D CMAKE_BUILD_TYPE=Release ../src

in your `build`-directory.

To compile the debug version again run

    cmake -D CMAKE_BUILD_TYPE=Debug ../src

You can also make different out-of-source build directories such as `debug/` or `release`.


###1.3 A note on compilers

In general it seems that the `gcc` error output is very verbose and [hard to read](http://clang.llvm.org/diagnostics.html). At the same time gcc seems to be slower/uses more memory.
For this reasons we advocate to use the `clang` compiler.

On Ubuntu you may install clang by 

    sudo apt-get install clang

You can switch your system compiler by running

    sudo update-alternatives --config c++

From now on the cmake build system will use clang and provide more comprehensible error messages.

###1.4 A note on make
Make is slow. However the build process can be made faster by using more threads by for example invoking

    make -j8

to run 8 threads in parallel.
However, the console output is still messy.
Hence, if you are one of the cool kids you may want to use ninja, on ubuntu based system you can install it by running

    sudo apt-get install ninja-build

You'll need to regenerate your build folder (remove it if you have generated makefiles before)

    rm -r build/
    mkdir build/ && cd build
    cmake -GNinja ../src
    ninja 

##2. Installing the library
To install the library simply run

    make install

if you want to specify the installation directory you can configure it via `CMAKE_INSTALL_PREFIX`.
    
    ccmake ../src

Be sure to configure (c) and generate (g) 

##3. Running unit-tests

After running make, you may run the unit-tests

    make test

Instead of running all test at once, you may run individual test by 

    ctest -R random_test

In case a test-case fails you can see the verbose output by running

    ctest -VV

###3.1 Disabling Unit Test Compilation
The compilation of Unit tests can be disabled by setting

    cmake -D TEST=false ../src

Analougously it may be enabled by

    cmake -D TEST=true ../src

You can create new unit-tests based on boost unit testing library in the tests/suites/ folder. Make sure to run `make test` again.


##4. Setting up an Eclipse CDT 4 Project

In the `build/`-directory execute

    cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE ../src

now there are Eclipse .project and .cproject files found in your build/ directory.

You can import them by `File`->`Import...`->`Existing Project in Workspace`, Select `Browse...` and choose your `build/` directory.
You may also want to install the [Eclipse CMake Editor](http://www.cthing.com/CMakeEd.asp) for editing any of the `CMakeList.txt` 
files more conveniently.

##4.1 Eclipse CDT Developer Quirks 

Note: This has been tested on Luna with CDT.

###4.1.1 `std::` includes are not properly resolved, erroneous highlighting
Solution: Right-Click onto the imported generated cmake geneial project -> C++ General / Preprocessor Include Paths -> Tab Providers -> Enable "CDT Cross GCC Built-in Compiler Settings"
In order to get c++11 support, change the "Command to get compiler specs" below to:
	
	${COMMAND} ${FLAGS} -E -P -std=c++11 -v -dD "${INPUTS}"

(basically add `-std=c++11` to the default params )

Then rebuild your Index. 
If that does not help search for the `__cplusplus` symbol and set it to `201103L`

###4.1.2 When using the build-in compile feature, make clutters the internal console

By default the imported project set `VERBOSE=1` for make, hence the cluttering.
To remove this right click onto the project settings -> *C/C++ Make Project* -> Tab: *Environment* -> Remove `VERBOSE` variable
(**Note:** setting it to `0` will not work.)


##5. Adding new Source files

Cmake will determine any new source files, however if it is a header file you may need to reinstall the library for other 
projects depending on the library to receive the new headers

##6. Create Doxygen Documentation
In order to generate Doxygen documentation make sure you'll have doxygen installed

    sudo apt-get install doxygen

Then in the `build/` directory run

    make doc

You will find the documentation in `build/doc/html/index.html`



##7. Running Benchmarks
Currently, we provide two test suites for running memory leak checks and profiling the calls.
The benchmarks are located in the `benchmark/`-folder.

###7.1 Preliminaries
You'll need valgrind on Debian-based distributions this may be installed by running
    
    sudo apt-get install valgrind

In addition, to be able to plot the callgraph you may run

    sudo apt-get install python graphviz

###7.2 Executing the benchmarks

For running the memcheck benchmark run 

    cd benchmarks
    ./valgrind-suite.sh

Analougously, for the callgrind benchmark run

    cd benchmarks
    ./callgrind-suite.sh

The callgrind suite produces a `callgrind.TIMESTAMP.log` whereas TIMESTAMP denotes the creation date. This log can be analyzed with tools such as `kcachegrind`. 
On an Ubuntu/Debian machine you may install kcachegrind by  

    sudo apt-get install kcachegrind


You can then see  the latest run using.

    kcachegrind `ls -tr callgrind.out.* | tail -1`

    

##8. Running the demo(s)
Currently, libGENEIAL is mainly a template-based library. Hence, it is essential that the demos compile to ensure 
that the library itself is compiling.
The demos are also compiled by either running the all-encompassing `make` or the more unequivocal `make demo{X}` , respectively where `{X}` denotes the demo number to build.
