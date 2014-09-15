#include "voxel_octree/MortonOctTree.h"
#include "gtest/gtest.h"

class MortonOctreeTests : public ::testing::Test
{
public:
    MortonOctreeTests()
    {
        m_octree = new MortonOctTree<10>();
        m_octree->AddOrphanNode(MNode(0,0,0));
        m_octree->AddOrphanNode(MNode(0,1,0));
        m_octree->AddOrphanNode(MNode(1,0,0));
        m_octree->AddOrphanNode(MNode(0,0,1));
        m_octree->SortLeafNodes();
        //m_octree->RebuildTree();
    }

    virtual ~MortonOctreeTests() {
        delete m_octree;
    }

protected:
    MortonOctTree<10> * m_octree;
};
