cmake_minimum_required (VERSION 2.6)
set(Python_ADDITIONAL_VERSIONS 3.4)
project (BUILD_LIBRARIES)

#Libs don't really need c++11?
#Causes build failure for googletest cause of strict ansi settings c++11 flag adds
#set(CMAKE_CXX_FLAGS "-fPIC -std=c++11 ${CMAKE_CXX_FLAGS}")

#libs that we want to build
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/stb_image/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/tinyxml2/src/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/glxw/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/glfw/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/freetype2/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/jsoncpp/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libsigc++/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/physfs/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/googletest/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libccd/")
