#include "Precomp.h"
#include "utility/Vector.h"
#include "utility/Logger.h"
#include "opengl/AABB.h"
#include "MortonOctree.h"
#include "BVoxLoader.h"

BVoxLoader::BVoxLoader(MortonOctTreePtr octree, Logger * log)
{
	m_log = log;
	m_octree = octree;
}

BVoxLoader::~BVoxLoader()
{

}

void BVoxLoader::ReadFile(const std::string &fileName)
{
	char * buf;
	uint32_t len;
	len=helpers::read(fileName,buf);

	m_log->log(LOG_LOG, "File len: %u", len);

	if(len==0)
		return;

	uint16_t * data = (uint16_t*)((void*)&buf[0]);
	uint32_t voxel_count = ((uint32_t*)data)[0];
	data++;
	data++;

	m_log->log(LOG_LOG, "Voxel counts: %u", voxel_count);

	for(int i = 0; i < voxel_count; i++)
	{
		uint16_t x = data[0], y = data[1], z = data[2];
		m_octree->AddOrphanNode(MNode(x,y,z));
		data+=3;
	}

	delete[] buf;
}


void BVoxLoader::WriteFile(const std::string &fileName)
{
	PHYSFS_file* f = PHYSFS_openWrite(fileName.c_str());

	if(!f)
	{
		m_log->log(LOG_LOG, "PHYSFS: Opening (%s) to write failed.\n", fileName.c_str());
		return;
	}

	auto nodes = m_octree->GetChildNodes();

	uint32_t p[3];
	uint32_t nodeCount = nodes.size();

	PHYSFS_write(f, (void*)&nodeCount, 4, 1);
	
	for (int i = 0; i < nodes.size(); ++i)
	{
		decodeMK(nodes[i].start,p[0],p[1],p[2]);
		PHYSFS_write(f, (void*)p, 4, 3);
	}
	
	m_log->log(LOG_LOG, "Saving level to file: %s\n",fileName.c_str());

	PHYSFS_close(f);
}