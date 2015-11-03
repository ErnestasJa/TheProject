#ifndef SECTION_VOXELIZER_H
#define SECTION_VOXELIZER_H

#include "IVoxelOutput.h"
#include "IPathSection.h"
#include "PathSectionStraight.h"
#include "PathSectionCorner.h"
#include "PathConfig.h"

///NOTE: Would be nicer to have different class for each type.

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
			case SectionType::Corner:
				return VoxelizeCorner((PathSectionCorner*)section.get());
			default:
				break;
		}

		return false;
	}

	bool VoxelizeCorner(PathSectionCorner * section)
	{	
		switch(section->GetStartAxis())
		{
			case SectionAxis::X:
			{
				glm::ivec3 sectionStart = section->GetStart(), sectionEnd = section->GetStart() + GetOpositeDirection(section->GetStartAxis())*PATH_WIDTH;
				for(int32_t x = sectionStart.x; x > sectionEnd.x; x--)
				{
					int32_t start = sectionStart.z - PATH_HALF_WIDTH;
					int32_t end = sectionStart.z + PATH_HALF_WIDTH;

					for(int32_t z = start; z < end; z++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);
					}
				}
				break;
			}
			case SectionAxis::NX:
			{
				glm::ivec3 sectionStart = section->GetStart(), sectionEnd = section->GetStart() + GetOpositeDirection(section->GetStartAxis())*PATH_WIDTH;
				for(int32_t x = sectionStart.x; x < sectionEnd.x; x++)
				{
					int32_t start = sectionStart.z - PATH_HALF_WIDTH;
					int32_t end = sectionStart.z + PATH_HALF_WIDTH;

					for(int32_t z = start; z < end; z++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);
					}
				}
				break;
			}
			case SectionAxis::Z:
			{
				glm::ivec3 sectionStart = section->GetStart(), sectionEnd = section->GetStart() + GetOpositeDirection(section->GetStartAxis())*PATH_WIDTH;
				for(int32_t z = sectionStart.z; z > sectionEnd.z; z--)
				{
					int32_t start = sectionStart.x - PATH_HALF_WIDTH;
					int32_t end = sectionStart.x + PATH_HALF_WIDTH;

					for(int32_t x = start; x < end; x++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);
					}
				}
				break;
			}
			case SectionAxis::NZ:
			{
				glm::ivec3 sectionStart = section->GetStart(), sectionEnd = section->GetStart() + GetOpositeDirection(section->GetStartAxis())*PATH_WIDTH;
				for(int32_t z = sectionStart.z; z < sectionEnd.z; z++)
				{
					int32_t start = sectionStart.x - PATH_HALF_WIDTH;
					int32_t end = sectionStart.x + PATH_HALF_WIDTH;

					for(int32_t x = start; x < end; x++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);
					}
				}
				break;
			}
			default:
				throw;
		}
		return true;
	}

	bool VoxelizeStraightSection(PathSectionStraight * section)
	{
		glm::ivec3 sectionStart = section->GetStart(), sectionEnd = section->GetEnd();
		
		switch(section->GetAxis())
		{
			case SectionAxis::X:
				for(int32_t x = sectionStart.x; x <= sectionEnd.x; x++)
				{
					int32_t start = sectionStart.z - PATH_HALF_WIDTH;
					int32_t end = sectionStart.z + PATH_HALF_WIDTH;

					for(int32_t z = start; z < end; z++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);

						if(	z == start || z == end-1)
							for(int32_t i = 1; i < PATH_WALL_HEIGHT; i++)
								m_voxOutput->AddVoxel(pos+glm::ivec3(0,i,0), VoxelOutputType::Wall);
					}
				}
			break;
			case SectionAxis::NX:
				for(int32_t x = sectionStart.x; x >= sectionEnd.x; x--)
				{
					int32_t start = sectionStart.z - PATH_HALF_WIDTH;
					int32_t end = sectionStart.z + PATH_HALF_WIDTH;

					for(int32_t z = start; z < end; z++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);

						if(	z == start || z == end-1)
							for(int32_t i = 1; i < PATH_WALL_HEIGHT; i++)
								m_voxOutput->AddVoxel(pos+glm::ivec3(0,i,0), VoxelOutputType::Wall);
					}
				}
			break;
			case SectionAxis::Z:
				for(int32_t z = sectionStart.z; z <= sectionEnd.z; z++)
				{
					int32_t start = sectionStart.x - PATH_HALF_WIDTH;
					int32_t end = sectionStart.x + PATH_HALF_WIDTH;

					for(int32_t x = start; x < end; x++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
						m_voxOutput->AddVoxel(pos, VoxelOutputType::Floor);

						if(	x == start || x == end-1)
							for(int32_t i = 1; i < PATH_WALL_HEIGHT; i++)
								m_voxOutput->AddVoxel(pos+glm::ivec3(0,i,0), VoxelOutputType::Wall);
					}
				}
			break;
			case SectionAxis::NZ:
				for(int32_t z = sectionStart.z; z >= sectionEnd.z; z--)
				{
					int32_t start = sectionStart.x - PATH_HALF_WIDTH;
					int32_t end = sectionStart.x + PATH_HALF_WIDTH;

					for(int32_t x = start; x < end; x++)
					{
						glm::ivec3 pos = glm::ivec3(x, sectionStart.y, z);
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