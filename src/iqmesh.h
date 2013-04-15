#pragma once
#include "iqm.h"
#include "geom.h"

/*
struct vertex
{
    vec3 position;
    vec3 normal;
    vec4 tangent[4];
    vec2 texcoord[2];
    uint8_t blendindex[4];
    uint8_t blendweight[4];
};
*/

struct material
{
    uint32_t texid;
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
    ///All data from file should be here, nullptr if not needed anymore.
	iqmheader data_header;
	char * data_buff;

    const char* texts;
    	//Information variables, will replace IQM ones with natives when the time comes. [MOST OF THEM ARE TEMPORARY]

	iqmmesh * submeshes;
	iqmvertexarray * vertexarrays; //IQM vertex array info
	iqmtriangle* triangles; //IQM triangle info(for indices)
	iqmjoint* joints; //IQM joint info
	iqmpose* poses; //IQM pose info
	iqmanim* anims; //IQM animation info
	iqmbounds* bounds; //IQM the bounding box

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
