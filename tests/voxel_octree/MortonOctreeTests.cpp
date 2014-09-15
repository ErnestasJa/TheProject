#include "precomp.h"
#include "MortonOctreeTests.h"
#include "voxel_octree/MortonOctTree.h"

// Tests that the Foo::Bar() method does Abc.


TEST_F(MortonOctreeTests, CanFindNodes) {
    MortonOctTree<10> * octree = m_octree;

    for(const MNode & node : octree->GetChildNodes())
    {
        std::cout << "Node start = " << node.start << std::endl;
    }

    EXPECT_TRUE(octree->CheckNode(0,0,0));
    EXPECT_TRUE(octree->CheckNode(1,0,0));
    EXPECT_TRUE(octree->CheckNode(0,1,0));
    EXPECT_TRUE(octree->CheckNode(0,0,1));
}


TEST_F(MortonOctreeTests, CullWorks) {
    MortonOctTree<10> * octree = m_octree;

    uint8_t culledSides = octree->GetVisibleSides(0,0,0,octree->GetChildNodes().begin());
    uint8_t correctSides = TOP | RIGHT | FRONT;

    EXPECT_EQ((int)correctSides, (int)culledSides);
}

