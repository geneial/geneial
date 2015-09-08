#!/usr/bin/env bash

mkdir build || return 1
cd build/ || return 1
cmake ../src || return 1

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

