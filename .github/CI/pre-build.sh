#!/bin/bash

## This script removes existing CMake (usually 3.12 on Ubuntu) and installs the latest:
##   - cmake
##   - g++
##   - ninja-build
## Arguments: 
##   1. (Optional) Whether to install clang/llvm (TRUE)

ENV="cmake g++-8 ninja-build"
CLANG=$1
if [[ "$CLANG" == "TRUE" ]]; then
	ENV="$ENV clang-8 lld-8"
	wget -O - https://apt.llvm.org/llvm-snapshot.gpg.key 2>/dev/null | sudo apt-key add -
fi

# Get latest keys for cmake, g++, etc
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' -y
sudo -E apt-add-repository -y "ppa:ubuntu-toolchain-r/test"

# Purge all existing cmake/clang installations
sudo apt-get purge {cmake,clang,lld}
sudo rm -rf /usr/{bin/cmake*,share/cmake*,local/bin/cmake*}
[[ "$CLANG" == "TRUE" ]] &&	sudo rm -rf /usr/{bin/clang*,share/clang*,local/bin/clang*}

# Install dependencies
sudo apt-get update
sudo apt-get install -y $ENV

if [[ "$CLANG" == "TRUE" ]]; then
	sudo ln -s /usr/bin/clang-8 /usr/bin/clang
	sudo ln -s /usr/bin/clang++-8 /usr/bin/clang++
fi
exit
