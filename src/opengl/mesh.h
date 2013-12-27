#ifndef MESH_H
#define MESH_H

struct ibuffer_object;
struct mesh
{
    uint32_t vao;
    std::vector<ibuffer_object*> buffers;

    mesh();

    void generate();
    void disable_empty_buffers();

    void free();
};


#endif // MESH_H
