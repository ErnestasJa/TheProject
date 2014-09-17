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
    MNode m_buildNodes[32][32][32];
    MortonOctTree<10> * m_octree;
    Map m_map;
    uint32_t m_level;

    MeshPtr CreateEmptyMesh();
    void ClearMesh(Mesh* mesh);
    void ClearBuildNodes();
    void SetBuildNode(const MNode & node);
    uint8_t GetVisibleBuildNodeSides(uint32_t x, uint32_t y, uint32_t z);
public:
    VoxMeshManager(MortonOctTree<10> * octree, uint32_t level = 5);
    virtual ~VoxMeshManager();

    Map & GetMeshes();
    void RebuildChunk(uint32_t chunk);
    void RenderAllMeshes();
    void GenAllChunks();
    void AddVoxelToMesh(Mesh* mesh, vector<MNode>::iterator nodeIt);
    void AddVoxelToMesh(Mesh* mesh, const MNode & node, uint8_t sides);
};

#endif // VOXMESHGENERATOR_H
