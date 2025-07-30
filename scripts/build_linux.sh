#!/usr/bin/bash

rm -rf build && \
    mkdir -p build && \
    cd build || exit $?

conan install .. \
    -s:a build_type=Debug \
    -s:a compiler.version=14 \
    --build missing \
    -o "&:build_gui=True" \
    -o "&:build_tests=True" || exit $?

cmake \
    --preset conan-debug \
    -DCMAKE_CXX_COMPILER=g++-14 \
    -DCMAKE_C_COMPILER=gcc-14 \
    .. || exit $?
