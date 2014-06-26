cmake_minimum_required (VERSION 2.6)
project (TESTPROJECT)

set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")


INCLUDE_DIRECTORIES(TESTPROJECT "${TESTPROJECT_SOURCE_DIR}/src"
  "${TESTPROJECT_SOURCE_DIR}/tests"
  "${TESTPROJECT_SOURCE_DIR}/libs/glm/"
  "${TESTPROJECT_SOURCE_DIR}/libs/glxw/include/"
  "${TESTPROJECT_SOURCE_DIR}/libs/physfs/"
  "${TESTPROJECT_SOURCE_DIR}/libs/glfw/include/"
  "${TESTPROJECT_SOURCE_DIR}/libs/libsigc++/"
  "${TESTPROJECT_SOURCE_DIR}/libs/glfw/include/"
  "${TESTPROJECT_SOURCE_DIR}/libs/freetype/include"
  "${TESTPROJECT_SOURCE_DIR}/libs/bullet/src/"
  "${TESTPROJECT_SOURCE_DIR}/libs/tinyxml2/src/"
  "${TESTPROJECT_SOURCE_DIR}/libs/irrklang-1.5.0/include/"
  "${TESTPROJECT_SOURCE_DIR}/libs/stb_image/"
  )

#Other libs that use CMake to build
add_subdirectory("${TESTPROJECT_SOURCE_DIR}/libs/stb_image/")
add_subdirectory("${TESTPROJECT_SOURCE_DIR}/libs/tinyxml2/src/")
add_subdirectory("${TESTPROJECT_SOURCE_DIR}/libs/glxw/")

link_directories ("${TESTPROJECT_SOURCE_DIR}/libs/physfs/lib")
link_directories ("${TESTPROJECT_SOURCE_DIR}/libs/libsigc++/lib")
link_directories ("${TESTPROJECT_SOURCE_DIR}/libs/glfw/lib")
link_directories ("${TESTPROJECT_SOURCE_DIR}/libs/bullet/lib/win32")
link_directories ("${TESTPROJECT_SOURCE_DIR}/libs/irrKlang-1.5.0/lib")
link_directories ("${TESTPROJECT_SOURCE_DIR}/libs/freetype/lib")


 


# Our project sources
add_executable(TESTPROJECT src/main.cpp

src/application/application.cpp
src/application/window.cpp

#src/tests/gui_and_fonts_application.cpp
src/tests/tinyxml_test_application.cpp
src/tests/kursinis/Objektas.cpp
src/tests/kursinis/kursinis.cpp
src/tests/game/physics/icollision_handler.cpp
src/tests/game/physics/physics_manager.cpp
src/tests/game/baigiamasis_quadcopter.cpp
src/tests/game/quadcopter.cpp
src/tests/game/test_game.cpp
src/tests/game/states/menu_state.cpp
src/tests/game/states/play_state.cpp
src/tests/game/states/state_manager.cpp


src/network/network_manager_win32.cpp


src/resources/iimage_loader.cpp
src/resources/image.cpp
src/resources/image_loader.cpp
src/resources/iqm_loader.cpp
src/resources/mesh_loader.cpp
src/resources/png_loader.cpp
src/resources/imesh_loader.cpp
src/resources/tgaloader.cpp
src/resources/shader_loader.cpp


src/opengl/buffer_object.cpp
src/opengl/frame_buffer_object.cpp
src/opengl/gl_object.cpp
src/opengl/ibuffer_object.cpp
src/opengl/mesh.cpp
src/opengl/opengl_util.cpp
src/opengl/render_buffer_object.cpp
src/opengl/texture.cpp


src/gui/font_renderer.cpp
src/gui/gui_button.cpp
src/gui/gui_checkbox.cpp
src/gui/gui_edit_box.cpp
src/gui/gui_element.cpp
src/gui/gui_environment.cpp
src/gui/gui_image.cpp
src/gui/gui_pane.cpp
src/gui/gui_skin.cpp
src/gui/gui_slider.cpp
src/gui/gui_static_text.cpp
src/gui/gui_window.cpp
src/gui/input_handler.cpp


src/scenegraph/isg_object.cpp
src/scenegraph/isg_render_queue.cpp
src/scenegraph/scenegraph.cpp
src/scenegraph/sg_aabb.cpp
src/scenegraph/sg_aabb_wireframe_object.cpp
src/scenegraph/sg_camera_object.cpp
src/scenegraph/sg_default_render_queue.cpp
src/scenegraph/sg_graphics_manager.cpp
src/scenegraph/sg_light_object.cpp
src/scenegraph/sg_line_object.cpp
src/scenegraph/sg_material.cpp
src/scenegraph/sg_mesh_object.cpp
src/scenegraph/sg_mvar.cpp
src/scenegraph/sg_quad.cpp
src/scenegraph/sg_scenegraph_loader.cpp
src/scenegraph/sg_shared_mat_vars.cpp
src/scenegraph/sg_skybox_object.cpp
src/scenegraph/sg_sprite.cpp

src/utility/logger.cpp
src/utility/timer.cpp

)

target_link_libraries (TESTPROJECT stb_image_lib 
  tinyxml2_lib
  physfs_win32
  freetype
  sigc++
  glfw3_win32
  glxw
  opengl32
  glu32
  gdi32
 
  BulletSoftBody
  BulletDynamics
  BulletCollision
  LinearMath
  
  irrKlang
  Ws2_32
  pthread) 