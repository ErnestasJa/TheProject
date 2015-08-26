#!/bin/sh

cd ./bin
export LD_LIBRARY_PATH=/home/serengeor/Coding/Project/src/engine/build:$LD_LIBRARY_PATH
exec ./VoxelOctree $1