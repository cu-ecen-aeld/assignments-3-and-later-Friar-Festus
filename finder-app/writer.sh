#!/usr/bin/bash

# First we check for the correct number of arguments,
# exiting if there are more or less than two.
if [ $# -ne 2 ]; then
  echo "There must be exactly two arguments."
  echo "Usage: $0 <writefile> <writestr>"
  exit 1
fi

# Create file, overwriting if it exists, and path if necessary
# Split out directory and filename from writefile
file="`basename "$1"`"
dir="`dirname "$1"`"

# Write writestr to writefile.
mkdir -p $dir
echo $2 > $1
