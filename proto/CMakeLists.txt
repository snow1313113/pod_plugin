cmake_minimum_required (VERSION 2.6)

project(pod_pb)

file(GLOB proto_file_list ${POD_PROTO}/*.proto)
foreach(proto_file ${proto_file_list})
    execute_process(COMMAND ${PROTOC_TOOL} --proto_path=${POD_PROTO} --proto_path=${PROTOBUF_DIR} --cpp_out=${POD_PROTO} ${proto_file} RESULT_VARIABLE retcode)
    if (NOT "${retcode}" STREQUAL "0")
        message(FATAL_ERROR "${BoldRed}Fatal Error${ColourReset}: protoc ${proto_file}")
    else()
        message(STATUS "${Green} ${proto_file} processed by protoc${ColourReset}")
    endif()
endforeach()

include_directories(.
    ${PROTOBUF_DIR}
    )

file(GLOB_RECURSE CC_SOURCE *.cc)
add_library(${PROJECT_NAME} ${CC_SOURCE})
set(LIBRARY_OUTPUT_PATH ${POD_LIB})
