#!/bin/sh

clang -I /home/eshaj/Documents/repo/klee/include/ -emit-llvm -c -g -O0 \
-Xclang -disable-O0-optnone $@
