cmake_minimum_required(VERSION 3.20)
project(score_grpc)

# Toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)

# Dependencies
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)

# Executable
add_executable(microservice
        src/microservice.cpp
        src/score.pb.cc
        src/score.grpc.pb.cc
)

# Link libraries
target_link_libraries(microservice
        PRIVATE
        gRPC::grpc++
        gRPC::grpc
        gRPC::gpr
        protobuf::libprotobuf
)