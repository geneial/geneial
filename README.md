<!---
This file uses markdown syntax, adhere when fiddling!
http://en.wikipedia.org/wiki/Markdown
-->

libGENEIAL - Elitism and RWTH: Yin and Yang
===========================================


##1. Building the library

###1.1 The easy way
There is a script that takes care of the building and installation process.
Simply run

    ./autobuilder.sh 

###1.2 The manual way

To build the genetic library use the following commands

    cd build/
    cmake ../src
    make

Make sure there is no Cmakecache.txt in the main directory, otherwise cmake does not work in out of source folder.

You'll find the static Libary in the `build/src/lib` library.

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
    
You can create new unit-tests based on boost unit testing libarary in the tests/suites/ folder. Make sure to run `make test` again.

##4. Setting up an Eclipse CDT 4 Project

In the `build/`-directory execute

    cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -D CMAKE_BUILD_TYPE=Debug ../src

now there are Eclipse .project and .cproject files found in your build/ directory.

You can import them by `File`->`Import...`->`Existing Project in Workspace`, Select `Browse...` and choose your `build/` directory.
You may also want to install the [Eclipse CMake Editor](http://www.cthing.com/CMakeEd.asp) for editing any of the `CMakeList.txt` 
files more conveniently.


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

In addition, to plot the callgraph you may run

    sudo apt-get install python graphviz

###7.2 Executing the benchmarks

For running the memcheck benchmark run 

    cd benchmarks
    ./valgrind-suite.sh

Analougously, for the callgrind benchmark run

    cd benchmarks
    ./callgrind-suite.sh


##8. Running the demo(s)
Currently, libGENEIAL is mainly a template-based library. Hence, it is essential that the demos compile to ensure 
that the library itself is compiling.
The demos are also compiled by either running the all-encompassing `make` or the more unequivocal `make demo{X}` , respectively where `{X}` denotes the demo number to build.


##99.TODOs
* mutation
* scaling
* multithreading 


* unit tests for like everything except random :)

* write demo programs
* write code documentation
* profit / world domination
