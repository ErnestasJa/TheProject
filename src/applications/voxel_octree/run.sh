#!/bin/sh

cd ./bin
export LD_LIBRARY_PATH=/home/senpai/Coding/TheProject/libs/linux:$LD_LIBRARY_PATH
exec ./VoxelOctree 2
