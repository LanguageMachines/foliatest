#!/bin/bash

echo "Running testaction script!"
valgrind ./src/foliatest
TEST_STAT=$?
echo $TEST_STAT > status.tmp
exit 0
