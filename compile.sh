#!/bin/bash

cmake ..
make
mv stabilize ..
#mv boundBoxTest ..
cd ..
./stabilize 2524 226 track
#./boundBoxTest