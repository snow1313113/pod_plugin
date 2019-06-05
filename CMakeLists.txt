cmake_minimum_required (VERSION 2.6)

# color variables
if(NOT WIN32)
  string(ASCII 27 Esc)
  set(ColourReset "${Esc}[m")
  set(ColourBold  "${Esc}[1m")
  set(Red         "${Esc}[31m")
  set(Green       "${Esc}[32m")
  set(Yellow      "${Esc}[33m")
  set(Blue        "${Esc}[34m")
  set(Magenta     "${Esc}[35m")
  set(Cyan        "${Esc}[36m")
  set(White       "${Esc}[37m")
  set(BoldRed     "${Esc}[1;31m")
  set(BoldGreen   "${Esc}[1;32m")
  set(BoldYellow  "${Esc}[1;33m")
  set(BoldBlue    "${Esc}[1;34m")
  set(BoldMagenta "${Esc}[1;35m")
  set(BoldCyan    "${Esc}[1;36m")
  set(BoldWhite   "${Esc}[1;37m")
endif()

SET(POD_PLUGIN_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})

# can use your own protobuf directory here
SET(PROTOBUF_ROOT_DIR ${POD_PLUGIN_ROOT_DIR}/third_party/protobuf)
SET(PROTOBUF_DIR ${PROTOBUF_ROOT_DIR}/src/)
SET(PROTOC_TOOL ${PROTOBUF_DIR}/protoc)
SET(PROTOBUF_LIB ${PROTOBUF_DIR}/.libs/)

# can use your own googletest directory here
SET(GTEST_ROOT ${POD_PLUGIN_ROOT_DIR}/third_party/googletest)
SET(GTEST_INCLUDE ${GTEST_ROOT}/googletest/include)
SET(GTEST_LIB ${GTEST_ROOT}/build/lib)

SET(POD_PROTO ${POD_PLUGIN_ROOT_DIR}/proto)
SET(POD_LIB ${POD_PLUGIN_ROOT_DIR}/lib)

SET(TEST_DIR ${POD_PLUGIN_ROOT_DIR}/test)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g -fPIC -std=c++11 -O2")

ADD_SUBDIRECTORY(proto)
ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)
