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
private:
	//Information variables
	std::vector<iqmmesh> meshes;
	std::vector<iqmjoint> joints;
	std::vector<iqmpose> poses;
	std::vector<iqmanim> anims;
	std::vector<iqmvertexarray> vertexarrays;
	std::vector<iqmbounds> bounds;
	
	GLuint vaoid;
	GLuint buffers[IQM_BUFFER_COUNT];
	std::vector<vertex> vertices;
	//Used for getting indices
	std::vector<iqmtriangle> triangles;
protected:

//Methods
public:
	iqmesh();
	virtual ~iqmesh();
	void load ( const unsigned char *data,const iqmheader &head );
private:
protected:
};
