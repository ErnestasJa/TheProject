# Install script for directory: E:/Coding/ZombieGameProject/libs/jsoncpp/include

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/jsoncpp")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/json" TYPE FILE FILES
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/assertions.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/autolink.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/config.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/features.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/forwards.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/json.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/reader.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/value.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/version.h"
    "E:/Coding/ZombieGameProject/libs/jsoncpp/include/json/writer.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

