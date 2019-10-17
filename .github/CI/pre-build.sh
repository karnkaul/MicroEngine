#!/bin/bash

ENV="cmake g++-8 ninja-build libx11-dev libxrandr-dev libgl1-mesa-dev libudev-dev libfreetype6-dev libflac-dev"

# Get latest keys for cmake, g++, etc
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | sudo apt-key add -
sudo apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' -y
sudo -E apt-add-repository -y "ppa:ubuntu-toolchain-r/test"

# Purge all existing cmake installations
sudo apt-get purge cmake
sudo apt-get update
sudo rm -rf /usr/bin/cmake*
sudo rm -rf /usr/share/cmake*
sudo rm -rf /usr/local/bin/cmake*

# Install dependencies
sudo apt-get install -y $ENV
