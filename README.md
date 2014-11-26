1. Building the library
=======================

To build the genetic library do the following

    cd build/
    cmake ..
    make

Make sure there is no Cmakecache.txt in the main directory, otherwise cmake does not work in out of source folder.

You'll find the static Libary in the build/src/lib library.

2. Running unit-tests
=====================

After running make, you may run the unit-tests

    make test

You can create new unit-tests based on boost unit testing libarary in the tests/suites/ folder.





TODO
====
* make install
* dynamic library
* demo programs
* documentation

