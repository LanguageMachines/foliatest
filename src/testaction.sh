#!/bin/bash

echo "Running testaction script!"
./src/foliatest
TEST_STAT=$?
echo $TEST_STAT > status.tmp
exit 0
