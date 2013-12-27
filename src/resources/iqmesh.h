#pragma once
#include "iqm.h"
#include "opengl/mesh.h"
#include "opengl/buffer_object.h"

class logger;
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
	buffer_object<glm::vec3>    positions;
	buffer_object<glm::vec2>    texcoords;
	buffer_object<glm::vec3>    normals;
	buffer_object<glm::vec4>    tangents;
	buffer_object<u8vec4>       bindexes;
	buffer_object<u8vec4>       bweights;
	buffer_object<glm::vec3>    colors;
	index_buffer_object<uint32_t> indices;

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
    logger *m_logger;
protected:

//Methods
public:
	iqmesh(logger *logger);
	virtual ~iqmesh();
	bool generate();

	void draw(bool whole);
	void set_frame(uint32_t frame);
	void set_interp_frame(float frame);

	void free();

private:
protected:
};
