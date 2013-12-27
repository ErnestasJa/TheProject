#ifndef MESH_H
#define MESH_H





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
    std::vector<sub_mesh>   sub_meshes; // if size == 0 it's single mesh, draw everything.

    mesh();

    void generate();
    void disable_empty_buffers();

    void render();

    void free();
};


#endif // MESH_H
