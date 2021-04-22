#!/bin/bash

cmake ..
make
mv stabilize ..
cd ..
./stabilize ./video/IMG_2524/IMG_2524_%04d.png