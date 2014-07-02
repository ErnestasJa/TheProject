#ifndef SG_SHARED_MAT_VARS_H
#define SG_SHARED_MAT_VARS_H

#include "scenegraph/SGMvar.h"

namespace sg
{

struct sg_shared_mat_vars
{
    sg_shared_mat_vars();

    sg_mvar<glm::mat4> view, proj, view_proj;
};

}

#endif // SG_SHARED_MAT_VARS_H
