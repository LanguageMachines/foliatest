#!/bin/bash

echo "Running testaction script!"
export FOLIAPATH=foliapy/folia-repo
export EXPECT=0
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib ./src/foliatest
TEST_STAT=$?
echo $TEST_STAT > status.tmp
exit 0
