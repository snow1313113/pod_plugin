cmake_minimum_required (VERSION 2.6)

project(pod_plugin_root)

include(common.cmake)

# can use your own protobuf directory here
SET(PROTOBUF_ROOT_DIR ${ROOT_DIR}/third_party/protobuf)
SET(PROTOBUF_DIR ${PROTOBUF_ROOT_DIR}/src/)
SET(PROTOC_TOOL ${PROTOBUF_DIR}/protoc)
SET(PROTOBUF_LIB ${PROTOBUF_DIR}/.libs/)

SET(POD_PROTO ${ROOT_DIR}/proto)
SET(POD_LIB ${ROOT_DIR}/lib)

ADD_SUBDIRECTORY(proto)
ADD_SUBDIRECTORY(src)
