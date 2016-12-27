#!/usr/bin/env bash
mpicxx -std=c++11 main.cpp -o lab5

mpirun -n 4 ./lab5 48 16