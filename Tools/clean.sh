#!/bin/bash

## == MicroEngine build directory cleaning script ==
# Cleans all outputs in `./Build`
#
## Usage:
# 	1. Invoke script from anywhere
#	2. Pass 'y' when prompted to delete 'Lib' (builds will require CMake regeneration afterwards)

ROOT=..

cd "$(dirname "$0")"
cd $ROOT

[[ ! -d Build ]] && exit
if [[ "$1" == "-s" || "$1" == "--silent" ]]; then
	CLEAN_LIB=TRUE
else
	read -p "Clean Lib? Will require CMake regeneration (n): " -n1 ANS
	echo
	[[ "$ANS" == "y" ]] && CLEAN_LIB=TRUE
fi

# Move Resources out temporarily
if [[ -d Build/Resources ]]; then
	mv Build/Resources ./
	RESOURCES_MOVED=TRUE
fi
if [[ "$CLEAN_LIB" != "TRUE" ]]; then
	mv Build/Lib ./
	LIB_MOVED=TRUE
fi

rm -rf Build/*
[[ "$RESOURCES_MOVED" == "TRUE" ]] && mv Resources Build
[[ "$LIB_MOVED" == "TRUE" ]] && mv Lib Build

echo -e "== 'Build' directory cleaned"
exit
