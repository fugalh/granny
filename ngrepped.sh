#!/bin/bash
# first argument is host (defaults to localhost)

# pipe in "ngrep -lt -W single ..."

# If you specify give an event, that is what we will print. If you don't, we
# will print the 8th word, i.e. the first word in the packet's data.
event=$1

egrep --line-buffered '^T' |\
if [ -n "$1" ]; then
    awk '{print "'$1'", $2, $3; fflush()}'
else
    awk '{print $8, $2, $3; fflush()}'
fi

# TODO UDP (^U and $8 becomes $7)
