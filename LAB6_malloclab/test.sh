#!/bin/bash

# This scripts tests the allocator with all available traces.

make clean && make
for FILE in traces/*.rep
do
    echo "Testing file: ${FILE}"
    ./mdriver -f $FILE
    echo ""
done