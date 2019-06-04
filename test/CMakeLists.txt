cmake_minimum_required (VERSION 2.6)

file(GLOB proto_file_list ${TEST_DIR}/proto/*.proto)
foreach(proto_file ${proto_file_list})
    execute_process(COMMAND ${PROTOC_TOOL}
        --proto_path=${POD_PROTO}
        --proto_path=${TEST_DIR}/proto
        --proto_path=${PROTOBUF_DIR}
        --cpp_out=${TEST_DIR}
        --pod_out=${TEST_DIR}
        --plugin=protoc-gen-pod=${POD_LIB}/pod_plugin
        ${proto_file} RESULT_VARIABLE retcode)
    if (NOT "${retcode}" STREQUAL "0")
        message(FATAL_ERROR "${BoldRed}Fatal Error${ColourReset}: protoc ${proto_file}")
    else()
        message(STATUS "${Green} ${proto_file} processed by protoc${ColourReset}")
    endif()
endforeach()
