#pragma once

class RpcServiceProxy;
class Poller;
class EventHandlerManager;
class ConnectionManager;
class RpcRequestHandler;
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
    Poller* _Poller;
    EventHandlerManager* EventHandlerMgr;
    ConnectionManager* ServerConnectionManager;
    RpcRequestHandler* RequestHandler;
    Reactor* _Reactor;
};
