#pragma once

#include <queue>
#include <array>

#include <common/Defines.hh>

class RpcServiceProxy;
class Poller;
class ThreadPool;
class EventHandlerManager;
class ServerConnectionManager;
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
    void RegisterService(RpcServiceProxy* Service);
    /**
     * Main entry
     */
    int Main(int argc, char* argv[]);

private:
    // workers
    RpcRequestHandler* RequestHandler;
    RpcResultSender* ResultSender;
    EventHandlerManager* EventHandlerMgr;
    Poller* poller;
    Reactor* reactor;
    ServerConnectionManager* ServerConnectionMgr;

    // container
    std::array<std::queue<std::pair<int,int>>, MAX_FILE_DESCRIPTORS> RpcResults;
};
