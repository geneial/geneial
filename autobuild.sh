#!/bin/bash
#@description simple guided noob frontend for building geneial
#@author Benedikt Wolters<benedikt.wolters@rwth-aachen.de>


BUILDDIR=build/


#show all commands from here
set -x
function cleanup_and_exit {
   rm -rf $BUILDDIR
   exit
}
#stop on any nonzero error

#TODO (bewo) optimize this script
#TODO (bewo) set -e not reliable?
#TODO (bewo) cleanup_and_exit is wrong (cd...)
set -e 
mkdir -p $BUILDDIR
cd $BUILDDIR

REPLY=
until [[ $REPLY = [RrDd] ]]; do
    read -p "Build [R]elease version or [D]ebug version?  [r/d]:" -n 1 -r
    echo    # (optional) move to a new line
done


BUILD_TYPE="Debug"
if [[ $REPLY = [Rr] ]]
then
    BUILD_TYPE="Release"
fi


#as long as we read invalid stuff prompt the user
REPLY=
until [[ $REPLY = [YyNn] ]]; do 
    read -p "Want to generate an Eclipse CDT4 Project? [y/n]" -n 1 -r
    echo    # (optional) move to a new line
done

projargs=()
if [[ $REPLY = [Yy] ]]; then
    projargs=( -G "Eclipse CDT4 - Unix Makefiles" -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=TRUE )
fi

cmake "${projargs[@]}" -D CMAKE_BUILD_TYPE=$BUILD_TYPE ../src

make VERBOSE=1


#as long as we read invalid stuff prompt the user
REPLY=
until [[ $REPLY = [YyNn] ]]; do
    read -p "Run unit test now? [y/n]" -n 1 -r
    echo    # (optional) move to a new line
done

#make install upon yes
if [[ $REPLY = [Yy] ]]
then
    if testout=$(make test); then
        echo "Test succeeded!"
    else
        REPLY=
        until [[ $REPLY = [YyNn] ]]; do
            read -p "Some test failed, continue? [y/n]:" -n 1 -r
            echo    # (optional) move to a new line
        done

        #make install upon yes
        if [[ $REPLY = [Nn] ]]
        then
            cleanup_and_exit
        fi
    fi
fi

echo "Build complete."

set +x

#as long as we read invalid stuff prompt the user
REPLY=
until [[ $REPLY = [YyNn] ]]; do
    read -p "Also wish to install?  [y/n]:" -n 1 -r
    echo    # (optional) move to a new line
done

#make install upon yes
if [[ $REPLY = [Yy] ]]
then
    sudo make install
fi

echo "DONE"
