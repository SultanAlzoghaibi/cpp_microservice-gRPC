cmake_minimum_required(VERSION 3.20)
project(score_grpc)

# Vcpkg toolchain path (works both in CLI and CLion)
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

set(CMAKE_CXX_STANDARD 17)

# Use CONFIG mode for vcpkg-installed packages
find_package(Protobuf CONFIG REQUIRED)
find_package(gRPC CONFIG REQUIRED)
include_directories(${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/include)
link_directories(${CMAKE_SOURCE_DIR}/vcpkg/installed/arm64-osx/lib)

add_executable(microservice
        src/microservice.cpp
        src/score.pb.cc
        src/score.grpc.pb.cc
)

target_link_libraries(score_server
        PRIVATE
        gRPC::grpc++
        gRPC::grpc
        gRPC::gpr
        protobuf::libprotobuf
)