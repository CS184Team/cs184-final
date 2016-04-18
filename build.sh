#!/bin/bash
mkdir -p build
cd build
cmake ..
make
SCRIPT="./$1 ../ply/happy_vrip.ply"
printf "[build.sh] Now executing:\n\t$SCRIPT\n"
eval $SCRIPT