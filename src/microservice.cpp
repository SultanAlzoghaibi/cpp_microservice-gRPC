#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "score.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

using score::ScoreService;
using score::CalculateScoreRequest;
using score::CalculateScoreResponse;

constexpr float INV_TABLE[21] = {
    0.0f,        // [0] — invalid, to avoid divide-by-zero
    1.0f,        // [1]
    0.5f,        // [2] //etc
    0.333333f,   // [3]
    0.25f,       // [4]
    0.2f,        // [5]
    0.166667f,   // [6]
    0.142857f,   // [7]
    0.125f,      // [8]
    0.111111f,   // [9]
    0.1f,        // [10]
    0.090909f,   // [11]
    0.083333f,   // [12]
    0.076923f,   // [13]
    0.071429f,   // [14]
    0.066667f,   // [15]
    0.0625f,     // [16]
    0.058824f,   // [17]
    0.055556f,   // [18]
    0.052632f,   // [19]
    0.05f        // [20]
};

class ScoreServiceImpl final : public ScoreService::Service {
    Status CalculateScore(ServerContext* context,
                          const CalculateScoreRequest* request,
                          CalculateScoreResponse* response) override {
        int total = 0;
        float sumSquares = 0.0f;
        int size = request->scores_size();

        for (const auto& pair : request->scores()) {
            int score = pair.second;
            total += score;
            sumSquares += score * score;
        }

        int avg = 0;
        float variance = 0.0f;

        if (size > 0) {
            float invSize = (size <= 20) * INV_TABLE[size] + (size > 20) * (1.0f / size);

            float mean = total * invSize;
            float meanSq = sumSquares * invSize;

            variance = meanSq - (mean * mean);  // ✅ no 'float' here

            avg = static_cast<int>(mean);
        }

        float multiplier = 1.0f;
        if (variance < 2.25f) {
            multiplier = 1.1f;
        } else if (variance > 9.0f) {
            multiplier = 0.9f;
        }

        int finalScore = static_cast<int>(avg * multiplier);
        response->set_final_score(finalScore);

        std::cout << "✅ Team " << request->team_id()
                  << " | Score = " << finalScore
                  << " | Avg = " << avg
                  << " | var = " << variance << std::endl
                  << " | mult = " << multiplier << std::endl;

        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    ScoreServiceImpl service;

    ServerBuilder builder;
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "🚀 gRPC server running on " << server_address << std::endl;
    server->Wait();
}

int main() {
    RunServer();
    return 0;
}