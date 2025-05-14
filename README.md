# 🧠 C++ gRPC Microservice — Team Score Calculator

This microservice is part of a distributed Slack survey system. It receives a list of team scores via gRPC and computes a **variance-adjusted average** to help measure team health more accurately. Built in C++ for maximum performance and efficiency.

## 🔧 Features

- 🚀 High-performance gRPC server written in C++
- 📊 Calculates mean and variance of input scores
- 🧮 Applies a multiplier based on variance:
  - Boosts consistent teams
  - Penalizes highly inconsistent input
- ⚡ Responds in ~0.098ms (measured on localhost after 100k warmups)
- 🛡️ Safe from divide-by-zero using a precomputed inverse table

## 🛠 Tech Stack

- C++17
- gRPC / Protocol Buffers
- Built with CMake + vcpkg for dependency management

## 📦 Build Instructions

1. **Install dependencies via `vcpkg`:**

```bash
git clone https://github.com/microsoft/vcpkg
./vcpkg/bootstrap-vcpkg.sh
./vcpkg/vcpkg install grpc protobuf
