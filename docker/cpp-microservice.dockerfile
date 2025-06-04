# cpp-microservice.dockerfile
FROM ubuntu:22.04
ENV CPLUS_INCLUDE_PATH=/usr/local/include

# Install build tools and gRPC dependencies
RUN apt-get update && apt-get install -y \
    build-essential cmake git curl unzip \
    protobuf-compiler libprotobuf-dev libprotoc-dev \
    && rm -rf /var/lib/apt/lists/*

# Install Protobuf
RUN git clone -b v24.1 https://github.com/protocolbuffers/protobuf.git && \
    cd protobuf && \
    git submodule update --init --recursive && \
    cmake -B build -S . -Dprotobuf_BUILD_TESTS=OFF && \
    cmake --build build --target install && \
    cd .. && rm -rf protobuf

# Install gRPC
RUN git clone -b v1.60.0 --recurse-submodules https://github.com/grpc/grpc.git && \
    cd grpc && \
    mkdir -p cmake/build && \
    cd cmake/build && \
    cmake -DgRPC_INSTALL=ON \
          -DgRPC_BUILD_TESTS=OFF \
          -DCMAKE_BUILD_TYPE=Release \
          ../.. && \
    make -j$(nproc) && \
    make install && \
    cd ../../../ && rm -rf grpc

# Copy project files and build your microservice
WORKDIR /app
COPY . .

RUN protoc -I=./proto ./proto/score.proto \
  --cpp_out=./src \
  --grpc_out=./src \
  --plugin=protoc-gen-grpc=/usr/local/bin/grpc_cpp_plugin

RUN cmake -S . -B build \
    -DCMAKE_PREFIX_PATH="/usr/local" \
 && cmake --build build

EXPOSE 50051

CMD ["./build/microservice"]