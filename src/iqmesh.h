#pragma once
#include "iqm.h"
struct vertexanim
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec2 texcoord;
	glm::detail::tvec4<unsigned char> blendindex;
	glm::detail::tvec4<unsigned char> blendweight;
};

struct vertexstatic
{
	
};
class iqmesh
{
//Variables
public:
	//Information variables
	std::vector<const char*> texts; //that's where names are put
	std::vector<iqmmesh*> meshes; //meshbuffer info
	std::vector<iqmvertexarray*> vertexarrays; //vertex array info
	std::vector<iqmtriangle*> triangles; //triangle info(for indices)
	std::vector<iqmjoint*> joints; //joint info
	std::vector<iqmpose*> poses; //pose info
	std::vector<iqmanim*> anims; //animation info
	std::vector<iqmbounds*> bounds; //the bounding box
	
	GLuint vaoid;
	GLuint buffers[IQM_BUFFER_COUNT];
	std::vector<vertexanim*> vertices;
	std::vector<unsigned int> indices;
private:
protected:

//Methods
public:
	iqmesh();
	virtual ~iqmesh();
	void generate();
private:
protected:
};
