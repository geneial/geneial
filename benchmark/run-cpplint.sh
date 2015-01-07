#!/bin/sh
for sourcefile in `find ../src/ -regex ".*\.[hc]pp" -or -regex ".*\.h"`; do 
    python ./cpplint.py --filter=-whitespace --extensions=hpp,cpp,h $sourcefile
done
