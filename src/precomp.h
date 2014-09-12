#ifndef _PRECOMP_H_
#define _PRECOMP_H_

// C/C++ SUITE INCLUDES
#undef __STRICT_ANSI__
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cstdio>
#include <stdint.h>
#include <iomanip>
#include <fstream>
#include <memory>
#include <vector>
#include <functional>
#include <map>
#include <unordered_map>

#include <string.h>
#include <string>

#include <sstream>
#include <stdarg.h>

#include <ctime>
#include <chrono>

#include <iterator>
#include <algorithm>

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
#include "utility/bit.h"
#include "utility/helpers.h"

/// DEFINES
#ifdef RELEASE
//#define RELEASE_FS
#endif

#ifdef DEBUG
#define LOG_OUTPUT
#endif

#endif
