#!/bin/bash

echo "Cleaning the project..."
cd "$(dirname "$0")"
cd ..
if [ -d "build" ]; then
    rm -rf build
fi
if [ -d "CMakeFiles" ]; then
    rm -rf CMakeFiles
fi
if [ -f "CMakeCache.txt" ]; then
    rm CMakeCache.txt
fi
if [ -f "Makefile" ]; then
    rm Makefile
fi
if [ -f "cmake_install.cmake" ]; then
    rm cmake_install.cmake
fi
if [ -f "test_fs_support_case_sensitivity" ]; then
    rm test_fs_support_case_sensitivity
fi
if [ -f "test_fs_support_CASE_sensitivity" ]; then
    rm test_fs_support_CASE_sensitivity
fi

echo "Project cleaned successfully!"