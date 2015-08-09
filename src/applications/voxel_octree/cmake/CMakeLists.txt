cmake_minimum_required (VERSION 2.6)
project (VoxelOctree)

include(CheckCXXCompilerFlag)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")

find_package(PythonLibs REQUIRED)
message(${PYTHON_INCLUDE_DIRS})
message(${PYTHON_LIBRARIES})

set(PROJECT_PATH "" CACHE FILEPATH "Path to root directory of project")
set(SRC_PATH "${PROJECT_PATH}/src/" )
set(APP_PATH "${PROJECT_PATH}/src/applications/voxel_octree" )
set(ENGINE_PATH "${SRC_PATH}/engine/" )
set(LIB_PATH "${PROJECT_PATH}/libs/" )
set(LIBBIN_PATH "${PROJECT_PATH}/libs/linux" )


include_directories(
	"${SRC_PATH}"
	"${APP_PATH}"
	"${ENGINE_PATH}"
	"${LIB_PATH}/boost/"
	"${LIB_PATH}/freetype/include"
	"${LIB_PATH}/glfw/include"
	"${LIB_PATH}/glm"
	"${LIB_PATH}/glxw/include"
	"${LIB_PATH}/physfs"
	"${LIB_PATH}/tinyxml2/src"
	"${LIB_PATH}/freetype2/include"
	"${LIB_PATH}/libsigc++"
	"${PYTHON_INCLUDE_DIRS}"
)

	
add_executable(VoxelOctree
	../main.cpp
	../VoxelOctreeApp.cpp
	../VoxelOctreeBenchmarkApp.cpp
	../player/Player.cpp
	../motree/VoxMeshManager.cpp
	../motree/MortonOctree.cpp
	../motree/MNode.cpp
	../motree/MNodeUtil.cpp
	../motree/CollisionManager.cpp
	../motree/CollisionInfo.cpp
	../motree/VoxelSide.cpp
	../py/cpputils.cpp
	../py/OctreeApplicationPy.cpp
	../py/OctreeUtils.cpp

)

#add_library(engine SHARED IMPORTED)
#set_target_properties(engine PROPERTIES IMPORTED_LOCATION "${LIBBIN_PATH}/libengine.so")
#set_target_properties(engine PROPERTIES IMPORTED_IMPLIB "${LIBBIN_PATH}/libengine.a")


target_link_libraries(VoxelOctree ${PYTHON_LIBRARIES} 
GL
GLU
X11
Xxf86vm
Xrandr
Xcursor
Xi
pthread
dl
z
"${LIBBIN_PATH}/libengined.so"
"${LIBBIN_PATH}/libsigc++.a"
"${LIBBIN_PATH}/libphysfs.a"
"${LIBBIN_PATH}/libglxw_glx.a"
"${LIBBIN_PATH}/libglxw.a"
"${LIBBIN_PATH}/libglfw3.a"
"${LIBBIN_PATH}/libfreetype.a"
"${LIBBIN_PATH}/libstb_image_lib.a"
"${LIBBIN_PATH}/libtinyxml2_lib.a"
"${LIBBIN_PATH}/libjsoncpp.a"
"${LIBBIN_PATH}/libboost_container.a"
)
