#!/usr/bin/env bash

function defCXX {
	export CC="$1"
	export CXX="$2"
}

function install {
	sudo apt-get -qq install --force-yes $*
}

sudo apt-get -qq update

install cmake doxygen libboost1.55-all-dev libstdc++-4.9-dev
install clang-3.5
defCXX clang-3.5 clang++-3.5

