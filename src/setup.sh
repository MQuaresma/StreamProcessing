#!/bin/bash

make clean
# Remove any leftover pipes or possible conflicting files
rm input
rm node*
# Compile
make
# Clean debug files
rm -r *.dSYM
rm *.o
pkill supervisor
