cmake_minimum_required(VERSION 3.0)

project(protobuf_test1 VERSION 0.1.0 LANGUAGES CXX)

find_package(protobuf PATHS "C:/Users/patri/Documents/Projects/Libs/cmake")

add_executable(${PROJECT_NAME} main.cpp message.pb.cc message.pb.h)
target_link_libraries(${PROJECT_NAME} protobuf::libprotobuf)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT message.pb.cc message.pb.h
	COMMAND protobuf::protoc ARGS --cpp_out=${CMAKE_CURRENT_BINARY_DIR} message.proto
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	DEPENDS message.proto)