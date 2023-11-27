#!/bin/sh

# First we check for the correct number of arguments,
# exiting if there are more or less than two.
if [ $# -ne 2 ]; then
  echo "There must be exactly two arguments."
  echo "Usage: $0 <directory> <searchstr>"
  exit 1
fi

# Check that the first argument is a directory.
if [ ! -d "$1" ]; then
  echo "First argument must be a directory."
  echo "Usage: $0 <directory> <searchstr>"
  exit 1
fi

# Get the number of files in the given directory.
num_files=$(find "$1" -type f | wc -l)

# Get the number of matching lines in files.
num_lines=$(grep "$2" "$1"/* | wc -l)
echo "The number of files are $num_files and the number of matching lines are $num_lines."
  