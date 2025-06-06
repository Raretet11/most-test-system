#!/bin/bash

# 1 - script name
# 2 - time limit
# 3 - memory limit
# 4 - error file
# 5 - output file
# 6 - test file
# 7 - meta file

isolate --cleanup

TIME_LIMIT_SEC=$(echo "scale=5; $2 / 1000" | bc)
MEMORY_LIMIT_KB=$3

SCRIPT_PATH=$(dirname "$BASH_SOURCE")

SOLUTION_PATH="${SCRIPT_PATH}/$1"
ERROR_PATH="${SCRIPT_PATH}/$4"
OUTPUT_PATH="${SCRIPT_PATH}/$5"
TEST_PATH="${SCRIPT_PATH}/$6"

ISOLATE_HOME=$(isolate --init)

cp "${TEST_PATH}" "${ISOLATE_HOME}/box/test.txt"
cp "${SOLUTION_PATH}" "${ISOLATE_HOME}/box/script.py"

ERROR_ISOLATE_PATH="${ISOLATE_HOME}/box/error.txt"
OUTPUT_ISOLATE_PATH="${ISOLATE_HOME}/box/output.txt"

isolate --time=${TIME_LIMIT_SEC} --stdout=output.txt --stderr=error.txt --stdin=test.txt --meta=${7} --env=HOME=/home/user --run /usr/bin/python3 script.py

cp ${OUTPUT_ISOLATE_PATH} ${OUTPUT_PATH}
cp ${ERROR_ISOLATE_PATH} ${ERROR_PATH}
