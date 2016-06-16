#!/usr/bin/env bash

function defCXX {
	export CC="$1"
	export CXX="$2"
}




cmake --version

if [[ ${USE} == "g++-4.8" ]]; then
	defCXX gcc-4.8 g++-4.8
elif [[ ${USE} == "g++-4.9" ]]; then
	defCXX gcc-4.9 g++-4.9
elif [[ ${USE} == "g++-5" ]]; then
	defCXX gcc-5 g++-5
elif [[ ${USE} == "clang++-3.4" ]]; then
	defCXX clang clang++
	export CXXFLAGS="-stdlib=libc++"
elif [[ ${USE} == "clang++-3.5" ]]; then
	defCXX clang-3.5 clang++-3.5
	export CXXFLAGS="-stdlib=libc++"
elif [[ ${USE} == "clang++-3.6" ]]; then
	defCXX clang clang++
elif [[ ${USE} == "clang++-3.7" ]]; then
	defCXX clang clang++
elif [[ ${USE} == "clang++-3.8" ]]; then
	defCXX clang clang++
fi
