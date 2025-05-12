cmake_minimum_required(VERSION 3.15)
project(score_grpc_server)

set(CMAKE_CXX_STANDARD 17)

# Use Homebrew include/lib paths explicitly
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(score_server
        src/microservice.cpp
        src/score.pb.cc
        src/score.grpc.pb.cc
)

target_link_libraries(score_server
        grpc++
        protobuf
)