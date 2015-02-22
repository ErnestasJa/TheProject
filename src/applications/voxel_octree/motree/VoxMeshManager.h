#ifndef VOXMESHGENERATOR_H
#define VOXMESHGENERATOR_H

#include "MortonOctree.h"
#include "opengl/Mesh.h"
#include "utility/Bit.h"
#include <boost/unordered_map.hpp>

typedef boost::unordered_map<uint32_t, MeshPtr>::iterator MapIterator;
typedef boost::unordered_map<uint32_t, MeshPtr> Map;

struct MaskNode;
class VoxMeshManager
{
private:
    MNode m_buildNodes[32][32][32];
    MortonOctTreePtr m_octree;
    Map m_map;
    uint32_t m_level;uint32_t faceCount;

    MeshPtr CreateEmptyMesh();
    void ClearMesh(Mesh* mesh);
    void ClearBuildNodes();
    void BuildSliceMask(uint32_t dir, uint32_t slice, MaskNode mask[32][32]);
    void BuildFacesFromMask(Mesh* mesh, int dim, int z, const glm::vec3 & offset, MaskNode mask[32][32], bool frontFaces);
    void AddFaceToMesh(Mesh* mesh, bool frontFace, uint32_t dir, uint32_t slice, glm::ivec2 start, glm::ivec2 dims, glm::vec3 offset, uint8_t color[3]);
    void GreedyBuildChunk(Mesh* mesh, const glm::vec3 & offset);
    void SetBuildNode(const MNode & node);
    uint8_t GetVisibleBuildNodeSides(uint32_t x, uint32_t y, uint32_t z);
    void GetBuildNode(MNode & n, uint32_t x, uint32_t y, uint32_t z);
public:
    VoxMeshManager(MortonOctTreePtr octree, uint32_t level = 5);
    virtual ~VoxMeshManager();

    Map & GetMeshes();
    void RebuildChunk(uint32_t chunk);
    void RenderAllMeshes();
    void GenAllChunks();
    void AddVoxelToMesh(Mesh* mesh, const MNode & node, uint8_t sides);
    void AddQuadToMesh(Mesh* mesh, const glm::vec3 * face, uint8_t color[3]);
};

#endif // VOXMESHGENERATOR_H
