#ifndef BVOX_LOADER_H
#define BVOX_LOADER_H


class MortonOctTree;
typedef std::shared_ptr<MortonOctTree> MortonOctTreePtr;

class Logger;

class BVoxLoader
{
public:
	BVoxLoader(MortonOctTreePtr octree, Logger * log);
	virtual ~BVoxLoader();

	void ReadFile(const std::string & fileName);
	void WriteFile(const std::string & fileName);

protected:
        MortonOctTreePtr m_octree;
        Logger * m_log;
        uint32_t Depth; /// just until we get rid of templated octree.
};

typedef std::shared_ptr<BVoxLoader> BVoxLoaderPtr;

#endif