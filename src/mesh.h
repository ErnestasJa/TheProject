#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include "precomp.h"

struct mesh
{
    uint32_t vao;
    std::vector<uint32_t> buffers;

    mesh(): vao(0)
    {

    }
};

#endif // MESH_H_INCLUDED
