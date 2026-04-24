#!/bin/bash

rm build/heartstone

cmake -S . -B build
cmake --build build

./build/heartstone
