cmake_minimum_required (VERSION 2.6)
project (VoxelOctree)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../" )
set(ENGINE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../engine" )
set(LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs" )
set(LIBBIN_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../libs/win32" )

include_directories(
	"${SRC_PATH}"
	"${ENGINE_PATH}"
	"${LIB_PATH}/boost/"
	"${LIB_PATH}/freetype/include"
	"${LIB_PATH}/glfw/include"
	"${LIB_PATH}/glm"
	"${LIB_PATH}/glxw/include"
	"C:/Python34/include"
)

	
add_executable(VoxelOctree
	../main.cpp
	../VoxelOctreeApp.cpp
	../VoxelOctreeBenchmarkApp.cpp
	../player/Player.cpp
	../motree/VoxMeshManager.cpp
	../motree/MortonOctree.cpp
	../motree/MNode.cpp
	../motree/CollisionManager.cpp
	../py/cpputils.cpp
	../py/OctreeApplicationPy.cpp
	../py/OctreeUtils.cpp
)

add_library(engine SHARED IMPORTED)
set_target_properties(engine PROPERTIES IMPORTED_LOCATION "${LIBBIN_PATH}")
set_target_properties(engine PROPERTIES IMPORTED_IMPLIB "${LIBBIN_PATH}/libengine.a")

add_library(python SHARED IMPORTED)
set_target_properties(python PROPERTIES IMPORTED_LOCATION "${LIBBIN_PATH}")
set_target_properties(python PROPERTIES IMPORTED_IMPLIB "${LIBBIN_PATH}/libpython34.a")

target_link_libraries(VoxelOctree python engine python engine)