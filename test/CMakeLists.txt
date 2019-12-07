cmake_minimum_required (VERSION 2.6)

include(../common.cmake)

# can use your own protobuf directory here
SET(PROTOBUF_ROOT_DIR ${ROOT_DIR}/../third_party/protobuf)
SET(PROTOBUF_DIR ${PROTOBUF_ROOT_DIR}/src)
SET(PROTOC_TOOL ${PROTOBUF_DIR}/protoc)
SET(PROTOBUF_LIB ${PROTOBUF_DIR}/.libs)

# can use your own googletest directory here
SET(GTEST_ROOT ${ROOT_DIR}/../third_party/googletest)
SET(GTEST_INCLUDE ${GTEST_ROOT}/googletest/include)
SET(GTEST_LIB ${GTEST_ROOT}/build/lib)

SET(POD_PROTO ${ROOT_DIR}/../proto)
SET(POD_LIB ${ROOT_DIR}/../lib)

project(plugin_test)

file(GLOB proto_file_list ${ROOT_DIR}/proto/*.proto)
foreach(proto_file ${proto_file_list})
    execute_process(COMMAND ${PROTOC_TOOL}
        --proto_path=${POD_PROTO}
        --proto_path=${ROOT_DIR}/proto
        --proto_path=${PROTOBUF_DIR}
        --cpp_out=${ROOT_DIR}
        --pod_out=${ROOT_DIR}
        --plugin=protoc-gen-pod=${POD_LIB}/pod_plugin
        ${proto_file} RESULT_VARIABLE retcode)
    if (NOT "${retcode}" STREQUAL "0")
        message(FATAL_ERROR "${BoldRed}Fatal Error${ColourReset}: protoc ${proto_file}")
    else()
        message(STATUS "${Green} ${proto_file} processed by protoc${ColourReset}")
    endif()
endforeach()

include_directories(${PROTOBUF_DIR}
    ${GTEST_INCLUDE}
    ${POD_PROTO}
    )

link_directories(${PROTOBUF_LIB} ${GTEST_LIB} ${POD_LIB})

aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})

target_link_libraries(${PROJECT_NAME}
    libgtest.a
    libpod_pb.a
    libprotobuf.a
    pthread
    )
