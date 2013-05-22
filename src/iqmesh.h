#pragma once
#include "iqm.h"
#include "mesh.h"

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
	iqmbounds* bounds; //IQM the bounding box

    /// data buffers
	glm::vec3* positions;
	glm::vec2* texcoords;
	glm::vec3* normals;
	glm::vec4* tangents;
	u8vec4 * bindexes;
	u8vec4 * bweights;
	glm::vec3 * colors;

    ///animation
    iqmjoint* joints; //IQM joint info
	iqmpose* poses; //IQM pose info
	iqmanim* anims; //IQM animation info

	glm::mat3x4 * frames;
	glm::mat3x4 * base_frame;
	glm::mat3x4 * inverse_base_frame;
	glm::mat3x4 * current_frame;
	uint16_t * frame_data;

	///GL buffers
	std::shared_ptr<mesh> glmesh;

private:
protected:

//Methods
public:
	iqmesh();
	virtual ~iqmesh();
	bool generate();

	void draw(bool whole);
	void set_frame(uint32_t frame);
	void set_interp_frame(float frame);

	void free();

private:
protected:
};
