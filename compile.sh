#!/bin/bash

cd /media/rpurcell/work/projects/stabilize/build
cmake ..
make
mv stabilize ..
cd ..
./stabilize