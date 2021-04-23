#!/bin/bash

cmake ..
make
mv stabilize ..
#mv boundBoxTest ..
cd ..
./stabilize ./video/IMG_2524/IMG_2524_%04d.png
#./boundBoxTest