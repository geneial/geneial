#!/bin/bash
#Author: Benedikt Wolters<mail@benedikt-wolters.de>
#Date: 2014-12-11
#Description: callgrind runner shorthand script
echo "Running callgrind"

#you may need 
#sudo apt-get install python graphviz
#for the callgraph
for DEMO in demo1
do
    LOGFILE=`mktemp`
    valgrind --callgrind-out-file=$LOGFILE --tool=callgrind ../build/demo/$DEMO/$DEMO
    echo 
    echo "Callgrind Done."
    echo "Callgraph Generation: "
    echo -n " Callgraph to Dot: "
    ./gprof2dot.py --format=callgrind --output=${DEMO}.dot $LOGFILE
    echo "written to $DEMO.dot"
    mv $LOGFILE ./callgrind.`date +"%s"`.log
    echo -n " Callgraph to Png: "
    dot -Tpng ${DEMO}.dot -o ${DEMO}.png
    echo "written to $DEMO.png"
done
