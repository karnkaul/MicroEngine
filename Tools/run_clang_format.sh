#!/bin/bash

# This script runs clang-format on all Source files (except `ThirdParty`)
# Requirements:
#  - clang-format

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
