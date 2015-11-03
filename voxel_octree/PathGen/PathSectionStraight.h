#ifndef PATH_SECTION_STRAIGHT_H
#define PATH_SECTION_STRAIGHT_H

#include "IPathSection.h"

class PathSectionStraight: public IPathSection
{
public:

	PathSectionStraight(glm::ivec3 start, uint32_t length, SectionAxis axis)
	: m_axis(axis), m_start(start)
	{
		glm::ivec3 endOffset;

		length = length -1;

		switch(m_axis)
		{
			case SectionAxis::X:
				endOffset = glm::ivec3(length,0,0);
			break;
			case SectionAxis::NX:
				endOffset = glm::ivec3(-length,0,0);
			break;
			case SectionAxis::Z:
				endOffset = glm::ivec3(0,0,length);
			break;
			case SectionAxis::NZ:
				endOffset = glm::ivec3(0,0,-length);
			break;
			default:
				throw;
		}

		m_end = m_start + endOffset;
	}

	virtual ~PathSectionStraight()
	{

	}
	
	virtual SectionType GetType()
	{
		return SectionType::Straight;
	}
	
	virtual SectionAxis GetAxis()
	{
		return m_axis;
	}

	glm::ivec3 GetStart()
	{
		return m_start;
	}

	glm::ivec3 GetEnd()
	{
		return m_end;
	}

	glm::ivec3 GetNextSectionStart(int32_t offsetNeeded)
	{
		glm::ivec3 endOffset;

		switch(m_axis)
		{
			case SectionAxis::X:
				endOffset = glm::ivec3(1,0,0);
			break;
			case SectionAxis::NX:
				endOffset = glm::ivec3(-1,0,0);
			break;
			case SectionAxis::Z:
				endOffset = glm::ivec3(0,0,1);
			break;
			case SectionAxis::NZ:
				endOffset = glm::ivec3(0,0,-1);
			break;
			default:
				throw;
		}

		return m_end + endOffset;
	}

private:
	glm::ivec3 m_start;
	glm::ivec3 m_end;
	SectionAxis m_axis;
};

#endif