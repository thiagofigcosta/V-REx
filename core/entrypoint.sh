#!/bin/bash

args="$@"

./vrex/src/bin/vrex $args

# TODO remove profiling
valgrind --leak-check=yes /vrex/src/bin/vrex