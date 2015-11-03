#ifndef PATH_MANAGER_H
#define PATH_MANAGER_H

#include "SectionVoxelizer.h"
#include "OctreeVoxelOutput.h"
#include <cstdlib>
#include <ctime>

class PathManager
{
public:
	PathManager(glm::ivec3 startOfPath)
	{
		m_pathStart = startOfPath;
		std::srand(std::time(0));
		m_output = share(new OctreeVoxelOutput());
		m_voxelizer = share(new SectionVoxelizer(m_output));
	}

	virtual ~PathManager()
	{

	}

	virtual void GenerateSections(uint32_t sectionCount)
	{

	}

	virtual void AppendRandomSection()
	{
		IPathSectionPtr previousSection = GetLastSection();
		SectionType type;

		if(previousSection && previousSection->GetType()==SectionType::Straight)
		{
			type = GetRandomSectionType();
		}
		else
		{
			type = SectionType::Straight;
		}

		AppendSection(type);
	}

	virtual void AppendSection(SectionType type)
	{
		m_output->SetColor(glm::ivec3(Rand(0,255),Rand(0,255),Rand(0,255)));

		IPathSectionPtr section, previousSection = GetLastSection();
		glm::ivec3 start = m_pathStart;

		if(previousSection)
		{
			start = previousSection->GetNextSectionStart(0);
		}

		SectionAxis startAxis = GetPreviousSectionEndAxis(previousSection);

		switch (type)
		{
			case SectionType::Straight:
			{
				section = share(new PathSectionStraight(start, 8, startAxis));
				break;
			}
			case SectionType::Corner:
			{
				startAxis = GetOpositeAxis(startAxis);
				SectionAxis endAxis = GetRandomCornerSectionEndAxis(startAxis);
				section = share(new PathSectionCorner(start, startAxis, endAxis));
				break;
			}
		}

		m_pathSections.push_back(section);
		m_voxelizer->Voxelize(section);
	}

	virtual SectionAxis GetOpositeAxis(SectionAxis axis)
	{
		switch(axis)
		{
			case SectionAxis::X:
				return SectionAxis::NX;
			case SectionAxis::NX:
				return SectionAxis::X;

			case SectionAxis::Z:
				return SectionAxis::NZ;
			case SectionAxis::NZ:
				return SectionAxis::Z;
		}
	}

	virtual SectionAxis GetPreviousSectionEndAxis(IPathSectionPtr previousSection)
	{
		SectionAxis endAxis;

		if(previousSection)
		{
			if(previousSection->GetType() == SectionType::Straight)
			{
				endAxis = static_cast<PathSectionStraight*>(previousSection.get())->GetAxis();
			}
			else if(previousSection->GetType() == SectionType::Corner)
			{
				endAxis = static_cast<PathSectionCorner*>(previousSection.get())->GetEndAxis();
			}
		}
		else
		{
			endAxis = SectionAxis::X;
		}

		return endAxis;
	}

	virtual SectionAxis GetRandomCornerSectionEndAxis(SectionAxis startAxis)
	{
		switch (startAxis)
		{
			case SectionAxis::X:
			case SectionAxis::NX:
				return Rand(0,1) ? SectionAxis::Z : SectionAxis::NZ;

			case SectionAxis::Z:
			case SectionAxis::NZ:
				return Rand(0,1) ? SectionAxis::X : SectionAxis::NX;
		}
	}

	virtual uint32_t Rand(uint32_t start, uint32_t end)
	{
		return start + (std::rand() % (uint32_t)(end - start + 1));
	}

	virtual IPathSectionPtr GetLastSection()
	{
		return m_pathSections.size() > 0 ? m_pathSections[m_pathSections.size()-1] : IPathSectionPtr(nullptr);
	}

	virtual SectionAxis GetRandomSectionAxis()
	{
		return (SectionAxis)Rand(1, 4);
	}

	virtual SectionType GetRandomSectionType()
	{
		return (SectionType)Rand(1, 2);
	}


private:
	vector<IPathSectionPtr> m_pathSections;
	glm::ivec3 m_pathStart;
	SectionVoxelizerPtr m_voxelizer;
	OctreeVoxelOutputPtr m_output;
};

typedef std::shared_ptr<PathManager> PathManagerPtr;

#endif	