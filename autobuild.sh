#!/bin/bash
#@author Benedikt Wolters<benedikt.wolters@rwth-aachen.de>

#show all commands from here
set -x

#stop on any nonzero error
set -e 
mkdir -p build/
cd build/
cmake -G"Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE -D CMAKE_BUILD_TYPE=Debug ../src
make
make test

set +x

echo "Build complete."

#as long as we read invalid stuff prompt the user
REPLY=
until [[ $REPLY =~ ^[YyNn]$ ]]; do
    read -p "Also wish to install?  [y/n]:" -n 1 -r
    echo    # (optional) move to a new line
done

#make install upon yes
if [[ $REPLY =~ ^[Yy]$ ]]
then
    sudo make install
fi

echo "DONE"
