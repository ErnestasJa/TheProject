#pragma once
#include "precomp.h"

class opengl_util
{
public:
    opengl_util();
    virtual ~opengl_util();

    bool load_extensions();
    bool check_and_output_errors();
};
