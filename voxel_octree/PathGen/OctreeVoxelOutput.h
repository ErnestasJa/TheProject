#ifndef OCTREE_VOXEL_OUTPUT
#define OCTREE_VOXEL_OUTPUT

#include "IVoxelOutput.h"
#include "VoxelOctreeApp.h"

class OctreeVoxelOutput: public IVoxelOutput
{
public:
	OctreeVoxelOutput()
	{
		m_color = glm::ivec3(255,255,255);
	}

	virtual	~OctreeVoxelOutput()
	{

	}

	void AddVoxel(const glm::ivec3 & voxel, VoxelOutputType type)
	{
		VoxelOctreeApp::Instance()->GetOctree()->AddOrphanNode(MNode(encodeMK(voxel.x,voxel.y,voxel.z), 1, m_color.x, m_color.y, m_color.z));
	}

	void SetColor(glm::ivec3 color)
	{
		m_color = color;
	}
	
private:
	glm::ivec3 m_color;
};


typedef std::shared_ptr<OctreeVoxelOutput> OctreeVoxelOutputPtr;
#endif