#!/usr/bin/env bash
mpicc -Iusr/local/include -std=c99 main.c -o lab4 -lm

mpirun -n 21 ./lab4