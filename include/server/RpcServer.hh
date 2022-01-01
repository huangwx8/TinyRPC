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
    struct Options
    {
        std::string ip_addr;
        int port;
        std::string log_path;
    };
public:
    RpcServer(Options);
    ~RpcServer();
    RpcServer(RpcServer&) = delete;
    RpcServer(RpcServer&&) = delete;
    /**
     * Register a new custom service object
     */
    void RegisterService(RpcServiceBase* Service);
/** 
     * Singleton get
     */
    static RpcServer& GetRpcServer(Options options);

private:
    /**
     * Initialize members and environment
     */
    void Initialize();
    /**
     * Main entry
     */
    int Main(int argc, char* argv[]);

    void ResetOneshot(int Fd);

    Options options;

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
