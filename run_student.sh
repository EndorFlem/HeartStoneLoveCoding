#!/bin/bash

rm -f build/student_solution

cmake -S . -B build
cmake --build build --target student_solution

./build/student_solution
