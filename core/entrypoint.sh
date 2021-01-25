#!/bin/bash

args="$@"

ulimit -s unlimited
export OMP_STACKSIZE=2G

./vrex/src/bin/vrex $args

# TODO remove profiling
valgrind --leak-check=yes /vrex/src/bin/vrex