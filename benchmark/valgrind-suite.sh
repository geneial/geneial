#!/bin/bash

valgrind -v --log-file="valgrind-`date +"%s"`.log" --leak-check=full --show-leak-kinds=all ../build/demo/demo1/demo1
