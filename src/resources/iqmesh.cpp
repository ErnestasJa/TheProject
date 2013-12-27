#include "precomp.h"

#include "iqmesh.h"
#include "math/util.h"
#include "resource_cache.h"
#include "utility/logger.h"
#include "opengl/buffer_object.h"

// TODO (serengeor#1#): Separate skeletal info from mesh


iqmesh::iqmesh(logger *logger)
{
    m_logger=logger;
    data_buff = NULL;
    frames = NULL;
	base_frame = NULL;
	inverse_base_frame = NULL;
	current_frame = NULL;
	texts = NULL;
	glmesh = share(new mesh());
	glmesh->buffers.resize(IQM_BUFFER_COUNT);
}

iqmesh::~iqmesh()
{
    free();
}

bool iqmesh::generate()
{

    glmesh->buffers[IQM_POSITION]   = &positions;
    glmesh->buffers[IQM_TEXCOORD]   = &texcoords;
    glmesh->buffers[IQM_NORMAL]     = &normals;
    glmesh->buffers[IQM_TANGENT]    = &tangents;
    glmesh->buffers[IQM_BLENDINDEXES]   = &bindexes;
    glmesh->buffers[IQM_BLENDWEIGHTS]   = &bweights;
    glmesh->buffers[IQM_COLOR]          = &colors;
    glmesh->buffers[IQM_INDICES]          = &indices;


    glmesh->disable_empty_buffers();
    glmesh->generate();

	return true;
}

void iqmesh::set_frame(uint32_t frame)
{
    const uint32_t offset = (frame%data_header.num_frames)*data_header.num_joints;

    for(uint32_t i = 0; i < data_header.num_joints; i++)
    {
        if(joints[i].parent >= 0) current_frame[i] = mul(current_frame[joints[i].parent],frames[offset+i]);
        else current_frame[i] = frames[offset+i];
    }
}

void iqmesh::set_interp_frame(float frame)
{
    if(data_header.num_frames <= 0) return;

    int frame1 = (int)floor(frame),
        frame2 = frame1 + 1;

    float frameoffset = frame - frame1;
    frame1 %= data_header.num_frames;
    frame2 %= data_header.num_frames;
    glm::mat3x4 *mat1 = &frames[frame1 * data_header.num_joints],
              *mat2 = &frames[frame2 * data_header.num_joints];
    // Interpolate matrixes between the two closest frames and concatenate with parent matrix if necessary.
    // Concatenate the result with the inverse of the base pose.
    // You would normally do animation blending and inter-frame blending here in a 3D engine.

    for(uint32_t i = 0; i < data_header.num_joints; i++)
    {
        glm::mat3x4 mat = mat1[i]*(1 - frameoffset) + mat2[i]*frameoffset;
        if(joints[i].parent >= 0) current_frame[i] = mul(current_frame[joints[i].parent],mat);
        else current_frame[i] = mat;
    }
}

void iqmesh::draw(bool whole)
{
	glmesh->render();
}

void iqmesh::free()
{
    if(data_buff) delete [] data_buff;
    if(frames) delete [] frames;
    if(base_frame) delete [] base_frame;
    if(inverse_base_frame) delete [] inverse_base_frame;
    if(current_frame) delete [] current_frame;

    if(texts) delete [] texts;
}
