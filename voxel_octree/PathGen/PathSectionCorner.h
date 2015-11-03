#ifndef PATH_SECTION_CORNER_H
#define PATH_SECTION_CORNER_H


#include "IPathSection.h"

class PathSectionCorner: public IPathSection
{
public:

	PathSectionCorner(glm::ivec3 start, SectionAxis startAxis, SectionAxis endAxis)
	: m_start(start), m_startAxis(startAxis), m_endAxis(endAxis)
	{
		
	}

	virtual ~PathSectionCorner()
	{

	}
	
	virtual SectionType GetType()
	{
		return SectionType::Corner;
	}
	
	virtual SectionAxis GetStartAxis()
	{
		return m_startAxis;
	}

	virtual SectionAxis GetEndAxis()
	{
		return m_endAxis;
	}

	glm::ivec3 GetStart()
	{
		return m_start;
	}

	glm::ivec3 GetNextSectionStart(int32_t offsetNeeded)
	{
		///FIX: Not yet fully working, need to visually debug..
		glm::ivec3 offset = GetOpositeDirection(m_startAxis) * (PATH_HALF_WIDTH - (m_endAxis==SectionAxis::X || m_endAxis== SectionAxis::Z));

		offset += GetDirection(m_endAxis) * ((m_endAxis==SectionAxis::NX || m_endAxis== SectionAxis::NZ) + PATH_HALF_WIDTH);

		return m_start + offset;
	}

private:
	glm::ivec3 m_start;
	SectionAxis m_startAxis, m_endAxis;
};


#endif  