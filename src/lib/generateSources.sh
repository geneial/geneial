#!/bin/bash
find -regex ".*\.\(h\|hpp\|cpp\)" | sed "s/\.\(h\|hpp\|cpp\)//" | uniq > ./Sources.txt
