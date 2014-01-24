#ifndef MESH_H
#define MESH_H

typedef std::vector<glm::mat3x4> frame;
typedef std::vector<frame> frame_vec;

struct bone
{
    int32_t parent;
    std::string name;
    glm::vec3 pos;
    glm::quat rot;
    glm::vec3 scale;
};
typedef std::vector<bone> bone_vec;

struct animation_info
{
    std::string name;
    uint32_t start, num;
    float framerate;
    bool loop;
};
typedef std::vector<animation_info> anim_info_vec;

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
    uint32_t start, num_indices;
};

struct ibuffer_object;
struct mesh
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
    std::vector<ibuffer_object*> buffers;
    std::vector<sub_mesh>   sub_meshes;
    animation * anim;

    mesh();
    virtual ~mesh();

    void generate();
    void disable_empty_buffers();

    void render();
    void render(uint32_t sub_mesh_index);

    void free();
};

typedef std::shared_ptr<mesh> mesh_ptr;

#endif // MESH_H
