#ifndef VOXMESHGENERATOR_H
#define VOXMESHGENERATOR_H

#include "MortonOctTree.h"
#include "opengl/Mesh.h"
#include "utility/bit.h"

class VoxMeshManager
{
private:
    MortonOctTree<10> * m_octree;

public:
    VoxMeshManager(MortonOctTree<10> * octree);
    virtual ~VoxMeshManager();
    void GenMesh(MeshPtr mesh);
    void AddVoxelToMesh(Mesh* mesh, vector<MNode>::iterator nodeIt);
};

#endif // VOXMESHGENERATOR_H
