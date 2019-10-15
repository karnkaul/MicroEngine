#!/bin/bash

## == MicroEngine clang formatter ==
# Runs clang-format on all Source files (except `ThirdParty`)
#
## Requirements:
#	- os.h
#	- clang-format
#
## Usage:
# 	1. Run the script to list the files it will format
# 	2. Pass '-r' to run clang-format

ROOT=..
SOURCE_DIR=Source
RUN=FALSE

[[ "$1" == "-r" || "$1" == "--run"* ]] && RUN=TRUE

cd "$(dirname "$0")"
. ./os.sh
cd $ROOT

if [[ ! -d $SOURCE_DIR ]]; then
	echo -e "  Error! $SOURCE_DIR does not exist!"
	exit 1
fi

FILES=$(find $SOURCE_DIR -name "*.h" -o -name "*.cpp")

if [[ $RUN == "TRUE" ]]; then
	$CLANG_FORMAT -i $FILES
	echo -e "  == Ran clang-format on:\n"
else
	echo -e "  -- Execute script again with -r to run clang-format on:\n"
fi
echo -e "${FILES}"

exit
