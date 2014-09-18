#ifndef MESH_H
#define MESH_H

#include "AABB.h"
#include "IBufferObject.h"
#include "BufferObject.h"

typedef vector<glm::mat3x4> frame;
typedef vector<frame> frame_vec;

struct bone
{
    int32_t parent;
    std::string name;
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
};
typedef vector<bone> bone_vec;

struct animation_info
{
    std::string name;
    uint32_t start, num;
    float framerate;
    bool loop;
};
typedef vector<animation_info> anim_info_vec;

struct animation
{
    frame_vec       frames;
    frame           current_frame;
    bone_vec        bones;
    anim_info_vec   info;

    void set_frame(uint32_t frame);
    void set_interp_frame(float frame);
};

struct sub_mesh
{
    std::string name;
    std::string material_name;
    uint32_t start, num_indices;
};

struct IBufferObject;
struct Mesh
{
    enum BUFFER_OBJECT_INDEX
    {
        POSITION     = 0,
        TEXCOORD     = 1,
        NORMAL       = 2,
        TANGENT      = 3,
        BLENDINDEXES = 4,
        BLENDWEIGHTS = 5,
        COLOR        = 6,
        INDICES      = 7,
        BUFFER_COUNT,
    };

    uint32_t vao;
    vector<IBufferObject*> buffers;
    vector<sub_mesh>   sub_meshes;
    animation * anim;
    AABB aabb;

    Mesh();
    virtual ~Mesh();

    void Init();
    void DisableEmptyBuffers();

    void Render();
    void Render(uint32_t sub_mesh_index);

    void render_lines();
    void render_triangle_strip();

    void RecalculateAABB();
    void UploadBuffers();
    void free();
};

typedef std::shared_ptr<Mesh> MeshPtr;

#endif // MESH_H
