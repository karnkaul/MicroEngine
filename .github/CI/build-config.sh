#!/bin/bash

## This script builds one or more configurations using cmake and ninja
## Arguments:
##   1. (Optional): Name of build configuration (Debug/RelWithDebInfo/etc)
##   2. (Optional): Whether to use clang++ (TRUE)

## Set config
CONFIG=$1
[[ -z $CONFIG ]] && CONFIG=Release

cmake --version
echo "ninja: $(ninja --version)"

## Set CXX_FLAGS (clang paths)
USE_CLANG=$2
[[ -z $CLANG ]] && CLANG=TRUE
if [[ $CLANG == "TRUE" ]]; then
	CXX_FLAGS="-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++"
	clang --version
fi
echo

## Build config
[[ ! -d out/$CONFIG ]] && mkdir -p out/$CONFIG
cmake -G Ninja . -Bout/$CONFIG $CXX_FLAGS -DCI_BUILD=1 -DCMAKE_BUILD_TYPE=$CONFIG
ninja -v -C out/$CONFIG

exit
