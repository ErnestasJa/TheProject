#ifndef IPATH_SECTION_H
#define IPATH_SECTION_H

enum class SectionType 
{ 
	Straight = 1,
	Corner,
};

enum class SectionAxis
{
	X = 1, 
	NX,
	Z,
	NZ
};

class IPathSection
{
public:
	virtual ~IPathSection(){}
	virtual SectionType GetType() = 0;
	virtual glm::ivec3 GetNextSectionStart(uint32_t offsetNeeded) = 0;
};

typedef std::shared_ptr<IPathSection> IPathSectionPtr;

#endif