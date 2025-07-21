#!/usr/bin/bash

conan profile detect > /dev/null
sed -i 's!compiler.version=13!compiler.version=14!' ~/.conan2/profiles/default
sed -i 's!build_type=Release!build_type=Debug!' ~/.conan2/profiles/default
sed -i 's!compiler.cppstd=gnu17!compiler.cppstd=23!' ~/.conan2/profiles/default

