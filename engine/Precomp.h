#ifndef _Precomp_H_
#define _Precomp_H_

#include "Config.h"

// C/C++ SUITE INCLUDES
//#include <stdio.h>
#include <iostream>
#include <cstdio>
#include <stdint.h>
#include <map>
#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>


#include <string.h>
#include <string>
#include <stdarg.h>

// OPENGL RELATED LIBS
#include <GLXW/glxw.h>

#include "GLFW/glfw3.h"

#include "glm.hpp"
#include "gtx/norm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/matrix_inverse.hpp"
#include "gtx/rotate_vector.hpp"

#include "gtc/type_ptr.hpp"

// OTHER LIBRARIES
#include "physfs.h"
#include "tinyxml2.h"
#include "sigc++/sigc++.h"

// PROJECT INCLUDES
#include "utility/Bit.h"
#include "utility/Helpers.h"

// BOOST INCLUDES
// jk, don't include boost here. pch size too big.


#endif
