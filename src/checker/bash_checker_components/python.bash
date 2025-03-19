#!/bin/bash

# 1 - script name
# 2 - time limit
# 3 - memory limit
# 4 - error file
# 5 - output file
# [6; +inf) - args for python

> "$4"

> "$5"

TIME_LIMIT_SEC=$(echo "scale=5; $2 / 1000" | bc)

MEMORY_LIMIT_KB=$3

ulimit -v $((MEMORY_LIMIT_KB + 1024 * 20))

echo "${@:6}" | timeout ${TIME_LIMIT_SEC} python "$1"

EXIT_CODE=$?
if [ $EXIT_CODE -eq 124 ]; then
  echo "Error: spent more then ${TIME_LIMIT_SEC} sec"
  exit 124
elif [ $EXIT_CODE -eq 137 ]; then
  echo "Error: spent more then ${MEMORY_LIMIT_KB} mb"
  exit 137
elif [ $EXIT_CODE -ne 0 ]; then
  echo "Error: code $EXIT_CODE"
  exit $EXIT_CODE
else
  exit 0
fi
