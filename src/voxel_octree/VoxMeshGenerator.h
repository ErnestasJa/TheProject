#ifndef VOXMESHGENERATOR_H
#define VOXMESHGENERATOR_H

#include "MortonOctTree.h"
#include "opengl/Mesh.h"
#include "utility/bit.h"



class VoxMeshGenerator
{
private:
    MortonOctTree<16> * m_octree;

public:
    VoxMeshGenerator(MortonOctTree<16> * octree);
    virtual ~VoxMeshGenerator();
    void GenMesh(MeshPtr mesh);
    void AddVoxelToMesh(Mesh* mesh, const MNode & node);
    inline uint8_t GetVisibleSides(uint32_t x, uint32_t y, uint32_t  z);
};

#endif // VOXMESHGENERATOR_H
