#pragma once

#include <common/RpcTypes.hh>

class RpcClient
{
public:
    RpcClient() = default;
    ~RpcClient();
    RpcClient(RpcClient&) = delete;
    RpcClient(RpcClient&&) = delete;
    /* initialize client */
    void Initialize(const char* ip, int port);
    /* client start running */
    int Main(int argc, char* argv[]);
    /* client send rpc request */
    void SendRequest(const RpcMessage& Message);
private:
    int connfd;
};
