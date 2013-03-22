#ifndef _PRECOMP_H_
#define _PRECOMP_H_

#include <iostream>
#include <stdio.h>
#include <stdint.h>
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
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#endif // _PRECOMP_H_
