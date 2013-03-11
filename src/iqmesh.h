#pragma once

#include "iqm.h"

struct vertex
{
    glm::vec3 position[3];
    glm::vec3 normal[3];
    glm::vec4 tangent[4];
    glm::vec2 texcoord[2];
    glm::detail::tvec4<unsigned char> blendindex;
    glm::detail::tvec4<unsigned char> blendweight;
};

class iqmesh
{
public:
	iqmesh();
	virtual ~iqmesh();
	
	void load(const unsigned char *data,const iqmheader &head);
private:
	std::vector<iqmmesh> meshes;
	std::vector<iqmjoint> joints;
	std::vector<iqmpose> poses;
	std::vector<iqmanim> anims;
	std::vector<iqmvertexarray> vertexarrays;
	std::vector<iqmbounds> bounds;
	
	GLuint buffers[IQM_BUFFER_COUNT];
	std::vector<vertex> vertices;
	std::vector<iqmtriangle> triangles;
protected:
	
};