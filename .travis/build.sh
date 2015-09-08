#!/usr/bin/env bash

function defCXX {
	export CC="$1"
	export CXX="$2"
}

mkdir build || return 1
cd build/ || return 1
cmake ../src || return 1


if [[ ${USE} == "g++-4.8" ]]; then
	install gcc-4.8 g++-4.8
	defCXX gcc-4.8 g++-4.8
elif [[ ${USE} == "g++-4.9" ]]; then
	install gcc-4.9 g++-4.9
	defCXX gcc-4.9 g++-4.9
elif [[ ${USE} == "clang++-3.5" ]]; then
	defCXX clang-3.5 clang++-3.5
elif [[ ${USE} == "clang++-3.6" ]]; then
	defCXX clang-3.6 clang++-3.6
elif [[ ${USE} == "clang++-3.7" ]]; then
	defCXX clang-3.7 clang++-3.7
fi



if [[ ${TASK} == "doxygen" ]]; then
	make doc
	
	git config --global user.email "benedikt.wolters@rwth-aachen.de"
	git config --global user.name "Travis Doxygen Builder"
	git clone https://${SECRET_ACCESS_TOKEN}@github.com/geneial/geneial.github.io.git
	cd geneial.github.io/ || return 1
	
	cp ../doc/html/* ./ || return 1
	git add . || return 1
	git commit -m "Updated documentation for geneial" || return 1
	git push origin master || return 1

else
	make -j1 || return 1
fi

