#!/usr/bin/env bash

# Count the number of times each word appears in a text file.
#
# For example, given a file containing:
#    predriver dichlamydeous predriver magnetoplasmadynamic undistrustful dichlamydeous undistrustful predriver undistrustful predriver
#
# The word counts are:
#    predriver            4
#    dichlamydeous        2
#    magnetoplasmadynamic 1
#    undistrustful        3

# Exit immediately if a command exits with a non-zero status.
set -e

if [ $# -eq 0 ]; then
    echo "usage: word_count.sh filename [filename ...]"
    exit 1
fi

if [ $# -eq 1 ]; then
    tr -c '[:alnum:]' '[\n*]' < $1 | sort | uniq -c
else
    for i in "$@"; do
        echo `basename $i`
        tr -c '[:alnum:]' '[\n*]' < $i | sort | uniq -c
        echo ""
    done
fi
