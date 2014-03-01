#include "precomp.h"
#include "sg_shared_mat_vars.h"

namespace sg
{

sg_shared_mat_vars::sg_shared_mat_vars()
{
    view = sg_mvar<glm::mat4>("view");
    proj = sg_mvar<glm::mat4>("proj");
    view_proj = sg_mvar<glm::mat4>("view_proj");

}

}
