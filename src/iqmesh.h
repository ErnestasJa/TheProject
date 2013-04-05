#pragma once
#include "iqm.h"
struct vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec4 tangent;
	glm::vec2 texcoord;
	glm::detail::tvec4<uint8_t> blendindex;
	glm::detail::tvec4<uint8_t> blendweight;
};

struct material
{
    GLuint texid;
};

struct iqsubmesh
{
	std::string name;
	material mat; //Future material implementations.
	uint32_t first_vertex,num_vertices;
	uint32_t first_index,num_indices;

	bool visible;
};

class iqmesh
{
//Variables
public:
	//Information variables, will replace IQM ones with natives when the time comes. [MOST OF THEM ARE TEMPORARY]
	std::vector<const char*> texts; //IQM that's where names are put
	std::vector<iqsubmesh> submeshes; //NATIVE submesh info, converted from iqm format
	std::vector<iqmvertexarray> vertexarrays; //IQM vertex array info
	std::vector<iqmtriangle> triangles; //IQM triangle info(for indices)
	std::vector<iqmjoint> joints; //IQM joint info
	std::vector<iqmpose> poses; //IQM pose info
	std::vector<iqmanim> anims; //IQM animation info
	std::vector<iqmbounds> bounds; //IQM the bounding box

	//geometry,buffers
	GLuint vaoid;
	GLuint buffers[IQM_BUFFER_COUNT];
	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec4> tangents;
	std::vector<glm::detail::tvec4<uint8_t>> bindexes;
	std::vector<glm::detail::tvec4<uint8_t>> bweights;
	std::vector<glm::vec3> colors;
	std::vector<uint32_t> indices;
private:
protected:

//Methods
public:
	iqmesh();
	virtual ~iqmesh();
	bool generate();
	void draw(bool whole);
private:
protected:
};
