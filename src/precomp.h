#ifndef _PRECOMP_H_
#define _PRECOMP_H_

#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdio.h>

#include <vector>
#include <string.h>

#include <GLXW/glxw.h>
#include <GL/glcorearb.h>
#ifdef __MINGW32__
#include <GLXW/glxw_wgl.h>
#include <GL/wglext.h>
#else
#include <GLXW/glxw_glx.h>
#include <GL/glxext.h>
#endif

#include <GL/glfw.h>

#include "glm.hpp"
#include "gtx/norm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "bit.h"

#endif
