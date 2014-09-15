#include "precomp.h"
#include <iostream>
#include "voxel_octree/MortonOctreeTests.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
