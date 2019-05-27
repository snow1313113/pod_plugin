cmake_minimum_required (VERSION 2.6)

project(pod_plugin)

file(GLOB_RECURSE SOURCE *.cpp *.cc)

include_directories(${PROJECT_SOURCE_DIR}
    ${PROTOBUF_DIR}
    ${POD_PROTO}
    )

add_executable(${PROJECT_NAME} ${SOURCE})

target_link_libraries(${PROJECT_NAME} 
    ${PROTOBUF_LIB}/libprotoc.a
    ${PROTOBUF_LIB}/libprotobuf.a
    ${POD_LIB}/libpod_pb.a
    )

set(EXECUTABLE_OUTPUT_PATH ${POD_LIB})
add_dependencies(${PROJECT_NAME} pod_pb)
