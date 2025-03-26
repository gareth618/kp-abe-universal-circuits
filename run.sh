#!/bin/bash
clang++ -std=c++20 pbc/*.cc *.cpp \
  -I/opt/homebrew/opt/gmp/include -L/opt/homebrew/opt/gmp/lib \
  -I/opt/homebrew/opt/pbc/include -L/opt/homebrew/opt/pbc/lib \
  -lgmp -lpbc && ./a.out && rm a.out
