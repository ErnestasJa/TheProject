#ifndef IPATH_SECTION_H
#define IPATH_SECTION_H

#include "PathConfig.h"

enum class SectionType 
{ 
	Straight = 1,
	Corner = 2,
};

enum class SectionAxis
{
	X = 1, 
	NX = 2,
	Z = 3,
	NZ = 4
};


inline glm::ivec3 GetDirection(SectionAxis axis)
{
	switch (axis)
	{
		case SectionAxis::X:
			return glm::ivec3(1,0,0);
		case SectionAxis::Z:
			return glm::ivec3(0,0,1);
		case SectionAxis::NX:
			return glm::ivec3(-1,0,0);
		case SectionAxis::NZ:
			return glm::ivec3(0,0,-1);
	}	
}

inline glm::ivec3 GetOpositeDirection(SectionAxis axis)
{
	return -1*GetDirection(axis);
}

class IPathSection
{
public:
	virtual ~IPathSection(){}
	virtual SectionType GetType() = 0;
	virtual glm::ivec3 GetNextSectionStart(int32_t offsetNeeded) = 0;
};

typedef std::shared_ptr<IPathSection> IPathSectionPtr;

#endif