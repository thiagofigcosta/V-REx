#!/bin/bash

args="$@"

gdb ./vrex/src/bin/vrex $args # TODO remove gdb

# TODO remove profiling
valgrind --leak-check=yes /vrex/src/bin/vrex