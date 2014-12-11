#!/bin/bash

valgrind -v --log-file="valgrind-demo1.log" --leak-check=full --show-leak-kinds=all ./build/demo/demo1/demo1
