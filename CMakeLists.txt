cmake_minimum_required (VERSION 2.6)
project (BUILD_LIBRARIES)

#Libs don't really need c++11?
#Causes build failure for googletest cause of strict ansi settings c++11 flag adds
#set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

#libs that we want to build
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/stb_image/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/tinyxml2/src/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/glxw/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/glfw/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/jsoncpp/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/libsigc++/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/physfs/")
add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/googletest/")

#No bullet yet..
#add_subdirectory("${BUILD_LIBRARIES_SOURCE_DIR}/libs/bullet/src/")