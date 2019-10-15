#!/bin/bash

## == MicroEngine build automation script ==
# Builds multiple MicroEngine CMake directories sequentially
#
## Requirements:
#	- os.h
#	- CMake 3.15+
#	- (Win64 ninja builds: vcvars64.bat loaded into shell environment)
#
## Usage:
# 	1. Invoke script from anywhere (Tab will autocomplete relative paths when in `MicroEngine` root)
# 	2. Pass relative paths (from `MicroEngine` root) to CMake build directories (separated by spaces)
# 	3. Pass -j{threads} for Makefiles, if desired
#	4. Eg: `./Tools/<script> out/MinGW_Debug out/MinGW_RelWithDebInfo out/build/x64-Debug -j8`

ROOT=..

cd "$(dirname "$0")"
. ./os.sh
cd $ROOT

if [[ $# < 1 ]]; then
	echo -e "\nError: \tMissing CMake build directory parameter!"
	echo -e "Usage: \t'$(basename "$0") <dir0> [dir1 dir2...] [-jT]'"
	echo -e "\twhere 'dirX' is where CMake generated a build environment (pass '.' to use 'out'),"
	echo -e "\tand 'T' is the number of threads (used for Makefiles only)\n"
	exit 1
fi

declare -a DIRS
IDX=0
for DIR in "${@}"; do
	if [[ $DIR == "-j"* ]]; then
		SUFFIX=$DIR
	elif [[ ! -d $DIR ]]; then
		echo -e "  $DIR does not exist!"
		exit 1
	else
		DIRS[IDX]=$DIR
		let IDX=$IDX+1
	fi
done

for DIR in "${DIRS[@]}"; do
	if [[ $IS_WIN64 == "TRUE" && -f $DIR/build.ninja ]]; then
		echo -e "Warning: \tAttempting MSVC build, will fail without vcvars64.bat loaded in PATH!\n"
	fi
	$CMAKE --build $DIR $SUFFIX
done

exit
