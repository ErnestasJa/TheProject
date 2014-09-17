#ifndef VOXMESHGENERATOR_H
#define VOXMESHGENERATOR_H

#include "MortonOctTree.h"
#include "opengl/Mesh.h"
#include "utility/bit.h"
#include <boost/unordered_map.hpp>

typedef boost::unordered_map<uint32_t, MeshPtr>::iterator MapIterator;
typedef boost::unordered_map<uint32_t, MeshPtr> Map;

class VoxMeshManager
{
private:
    MortonOctTree<10> * m_octree;
    Map m_map;
    uint32_t m_level;

    MeshPtr CreateEmptyMesh();
    void ClearMesh(Mesh* mesh);
public:
    VoxMeshManager(MortonOctTree<10> * octree, uint32_t level = 5);
    virtual ~VoxMeshManager();

    Map & GetMeshes();
    void RebuildChunk(uint32_t chunk);
    void RenderAllMeshes();
    void GenAllChunks();
    void AddVoxelToMesh(Mesh* mesh, vector<MNode>::iterator nodeIt);
};

#endif // VOXMESHGENERATOR_H
