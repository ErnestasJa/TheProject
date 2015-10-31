#ifndef IPATH_FUNCTION_H
#define IPATH_FUNCTION_H

class IPathFunction
{
public:
	virtual ~IPathFunction() {};
	virtual vector<glm::ivec3> GeneratePath() = 0;
};

#endif