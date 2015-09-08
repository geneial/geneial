#!/usr/bin/env bash

mkdir build || return 1
cd build/ || return 1
cmake ../src || return 1
make

