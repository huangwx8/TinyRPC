#pragma once

#include <queue>
#include <array>

#include <common/Defines.hh>
#include <common/RpcTypes.hh>

class RpcServiceBase;
class Poller;
class ThreadPool;
class EventHandlerManager;
class ServerTransport;
class RpcRequestHandler;
class RpcResultSender;
class Reactor;

class RpcServer
{
public:
    RpcServer();
    ~RpcServer();
    RpcServer(RpcServer&) = delete;
    RpcServer(RpcServer&&) = delete;
    
    /**
     * Initialize members and environment
     */
    void Initialize();
    /**
     * Register a new custom service object
     */
    void RegisterService(RpcServiceBase* Service);
    /**
     * Main entry
     */
    int Main(int argc, char* argv[]);

private:
    void ResetOneshot(int Fd);

    // workers
    RpcRequestHandler* RequestHandler;
    RpcResultSender* ResultSender;
    EventHandlerManager* EventHandlerMgr;
    Poller* poller;
    Reactor* reactor;
    ServerTransport* Transport;

    // container
    std::array<std::queue<RpcResult>, MAX_FILE_DESCRIPTORS> PendingResults;
};
