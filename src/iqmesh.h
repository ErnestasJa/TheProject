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

struct u8vec4{uint8_t v[4];};


class iqmesh
{
//Variables
public:
	iqmheader data_header;
	uint8_t * data_buff;

    const char** texts;

    ///metadata buffers
	iqmmesh * submeshes;
	iqmvertexarray * vertexarrays; //IQM vertex array info
	iqmtriangle* triangles; //IQM triangle info(for indices)
	iqmjoint* joints; //IQM joint info
	iqmpose* poses; //IQM pose info
	iqmanim* anims; //IQM animation info
	iqmbounds* bounds; //IQM the bounding box

    /// data buffers
	vec3* positions;
	vec2* texcoords;
	vec3* normals;
	vec4* tangents;
	u8vec4 * bindexes;
	u8vec4 * bweights;
	vec3 * colors;

    ///animation
	mat3x4 * frames;
	mat3x4 * base_frame;
	mat3x4 * inverse_base_frame;
	mat3x4 * current_frame;
	uint16_t * frame_data;

	///GL buffers
	GLuint vaoid;
	GLuint buffers[IQM_BUFFER_COUNT];

private:
protected:

//Methods
public:
	iqmesh();
	virtual ~iqmesh();
	bool generate();
	void draw(bool whole);

	void set_frame(float frame);

private:
protected:
};
