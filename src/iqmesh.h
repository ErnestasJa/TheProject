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

struct iqsubmesh
{
	std::string name;
	//material material; Future material implementations.
	uint32_t first_vertex,num_vertices;
	uint32_t first_index,num_indices;
	
	bool visible;
};

class iqmesh
{
//Variables
public:
	//Information variables
	std::vector<const char*> texts; //that's where names are put
	std::vector<iqsubmesh> submeshes; //native submesh info, converted from iqm format
	std::vector<iqmvertexarray*> vertexarrays; //vertex array info
	std::vector<iqmtriangle*> triangles; //triangle info(for indices)
	std::vector<iqmjoint*> joints; //joint info
	std::vector<iqmpose*> poses; //pose info
	std::vector<iqmanim*> anims; //animation info
	std::vector<iqmbounds*> bounds; //the bounding box
	
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
