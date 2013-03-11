#pragma once
#include "iqm.h"
struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec2 texcoord;
	glm::detail::tvec4<unsigned char> blendindex;
	glm::detail::tvec4<unsigned char> blendweight;
};
class iqmesh
{
//Variables
public:
	//Information variables
	std::vector<const char*> texts; //that's where names are put
	std::vector<iqmmesh*> meshes; //meshbuffers
	std::vector<iqmvertexarray*> vertexarrays;
	std::vector<iqmtriangle*> triangles;
	std::vector<iqmjoint*> joints;
	std::vector<iqmpose*> poses;
	std::vector<iqmanim*> anims;
	std::vector<iqmbounds*> bounds;
	
	GLuint vaoid;
	GLuint buffers[IQM_BUFFER_COUNT];
	std::vector<vertex*> vertices;
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
