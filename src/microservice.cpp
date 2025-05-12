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

class ScoreServiceImpl final : public ScoreService::Service {
    Status CalculateScore(ServerContext* context,
                          const CalculateScoreRequest* request,
                          CalculateScoreResponse* response) override {
        int total = 0;
        for (const auto& pair : request->scores()) {
            total += pair.second;
        }
        int avg = request->scores_size() > 0 ? total / request->scores_size() : 0;
        response->set_final_score(avg);

        std::cout << "✅ Received score request for team " << request->team_id()
                  << " | Final score = " << avg << std::endl;

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