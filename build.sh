#!/bin/bash

conan install . --build=missing
cd build/Release/generators
cmake ../../..
cmake --build .