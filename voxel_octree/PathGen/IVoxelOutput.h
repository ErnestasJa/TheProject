#ifndef IVOXEL_OUTPUT_H
#define IVOXEL_OUTPUT_H

enum class VoxelOutputType
{
	Floor = 1,
	Wall
};

class IVoxelOutput
{
public:
	virtual	~IVoxelOutput(){}
	virtual void AddVoxel(const glm::ivec3 & voxel, VoxelOutputType type) = 0;
private:

};

typedef std::shared_ptr<IVoxelOutput> IVoxelOutputPtr;

#endif