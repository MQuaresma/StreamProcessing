#!/bin/bash

# Remove any leftover pipes
rm node*
rm log
# Compile
make
# Clean debug files
rm -r *.dSYM
