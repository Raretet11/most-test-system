#!/bin/bash

# 1 - script name
# 2 - time limit
# 3 - memory limit
# 4 - error file
# 5 - output file
# [6; +inf) - args for python

> $4

> $5

TIME_LIMIT_MSEC=$2

MEMORY_LIMIT_MB=$3

ulimit -v $((MEMORY_LIMIT_MB * 1024))

timeout --kill-after=0 ${TIME_LIMIT_MSEC / 1000} python "$1" "${@:6}"

EXIT_CODE=$?
if [ $EXIT_CODE -eq 124 ]; then
  echo "Error: spent more then ${TIME_LIMIT_MSEC} sec"
  exit 124
elif [ $EXIT_CODE -eq 137 ]; then
  echo "Error: spent more then ${MEMORY_LIMIT_MB} mb"
  exit 137
elif [ $EXIT_CODE -ne 0 ]; then
  echo "Error: code $EXIT_CODE"
  exit $EXIT_CODE
fi
