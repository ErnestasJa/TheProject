#!/bin/sh

cd ./bin
export LD_LIBRARY_PATH=/home/serengeor/Coding/Project/bin/Debug:$LD_LIBRARY_PATH
exec optirun ./VoxelOctree $1