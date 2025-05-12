cmake_minimum_required(VERSION 3.20)
project(score_grpc)

set(CMAKE_CXX_STANDARD 17)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)

# Optional but safe:
find_package(absl REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${gRPC_INCLUDE_DIRS})

add_executable(score_server
        src/microservice.cpp
        src/score.grpc.pb.cc
        src/score.pb.cc
)

target_link_libraries(score_server
        gRPC::grpc++
        protobuf::protobuf
        absl::log_internals
        absl::strings
        absl::status
        absl::cord
)