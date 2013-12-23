#pragma once

struct mesh
{
    uint32_t vao;
    std::vector<uint32_t> buffers;

    mesh(): vao(0)
    {

    }

    void free()
    {

    }
};
