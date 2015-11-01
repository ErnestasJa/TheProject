#ifndef SECTION_VOXELIZER_H
#define SECTION_VOXELIZER_H

#include "IVoxelOutput.h"
#include "IPathSection.h"
#include "PathSectionStraight.h"

///NOTE: Would be nicer to have different class for each type.

#define PATH_HALF_WIDTH 3
#define PATH_WALL_HEIGHT 3

class SectionVoxelizer
{
public:
	SectionVoxelizer(IVoxelOutputPtr voxOutput)
	{
		m_voxOutput = voxOutput;
	}

	virtual ~SectionVoxelizer(){}

	bool Voxelize(IPathSectionPtr section)
	{

		switch(section->GetType())
		{
			case SectionType::Straight:
				return VoxelizeStraightSection((PathSectionStraight*)section.get());
			default:
				break;
		}

		return false;
	}

	bool VoxelizeStraightSection(PathSectionStraight * section)
	{
		glm::ivec3 sstart = section->GetStart(), send = section->GetEnd();
		
		switch(section->GetAxis())
		{
			case SectionAxis::X:
				for(int32_t x = sstart.x; x < send.x; x++)
				{
					int32_t start = sstart.z - PATH_HALF_WIDTH;
					int32_t end = sstart.z + PATH_HALF_WIDTH;

					for(int32_t z = start; z < end; z++)
					{
						glm::ivec3 pos = glm::ivec3(x, sstart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);

						if(	z == start || z == end-1)
							for(int32_t i = 1; i < PATH_WALL_HEIGHT; i++)
								m_voxOutput->AddVoxel(pos+glm::ivec3(0,i,0), VoxelOutputType::Wall);
					}
				}
			break;
			case SectionAxis::NX:
				for(int32_t x = sstart.x; x > send.x; x--)
				{
					int32_t start = sstart.z - PATH_HALF_WIDTH;
					int32_t end = sstart.z + PATH_HALF_WIDTH;

					for(int32_t z = start; z < end; z++)
					{
						glm::ivec3 pos = glm::ivec3(x, sstart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);

						if(	z == start || z == end-1)
							for(int32_t i = 1; i < PATH_WALL_HEIGHT; i++)
								m_voxOutput->AddVoxel(pos+glm::ivec3(0,i,0), VoxelOutputType::Wall);
					}
				}
			break;
			case SectionAxis::Z:
				for(int32_t z = sstart.z; z < send.z; z++)
				{
					int32_t start = sstart.x - PATH_HALF_WIDTH;
					int32_t end = sstart.x + PATH_HALF_WIDTH;

					for(int32_t x = start; x < end; x++)
					{
						glm::ivec3 pos = glm::ivec3(x, sstart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);

						if(	x == start || x == end-1)
							for(int32_t i = 1; i < PATH_WALL_HEIGHT; i++)
								m_voxOutput->AddVoxel(pos+glm::ivec3(0,i,0), VoxelOutputType::Wall);
					}
				}
			break;
			case SectionAxis::NZ:
				for(int32_t z = sstart.z; z > send.z; z--)
				{
					int32_t start = sstart.x - PATH_HALF_WIDTH;
					int32_t end = sstart.x + PATH_HALF_WIDTH;

					for(int32_t x = start; x < end; x++)
					{
						glm::ivec3 pos = glm::ivec3(x, sstart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);

						if(	x == start || x == end-1)
							for(int32_t i = 1; i < PATH_WALL_HEIGHT; i++)
								m_voxOutput->AddVoxel(pos+glm::ivec3(0,i,0), VoxelOutputType::Wall);
					}
				}
			break;
		}
	}


private:
	IVoxelOutputPtr m_voxOutput;
};

typedef std::shared_ptr<SectionVoxelizer> SectionVoxelizerPtr;


#endif