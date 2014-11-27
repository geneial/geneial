<!---
This file uses markdown syntax, adhere when fiddling!
-->

OASysGL - Elitism and RWTH: Yin and Yang
========================================


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

You can create new unit-tests based on boost unit testing libarary in the tests/suites/ folder.

##4. Setting up an Eclipse CDT 4 Project

In the `build/`-directory execute

    cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -D CMAKE_BUILD_TYPE=Debug ../src

now there are Eclipse .project and .cproject files found in your build/ directory.

You can import them by `File`->`Import...`->`Existing Project in Workspace`, Select `Browse...` and choose your `build/` directory.
You may also want to install the [Eclipse CMake Editor](http://en.wikipedia.org/wiki/Markdown) for editing any of the `CMakeList.txt` 
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

##99.TODOs
* write the actual library 
* write demo programs
* write code documentation
* profit / world domination
