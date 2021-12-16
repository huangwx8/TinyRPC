#pragma once

#include <atomic>

#include <common/RpcTypes.hh>

class Poller;
class RpcServiceProxy;
class EventHandlerRouter;
class ClientConnectionManager;
class CallbacksHandler;

class RpcClient
{
public:
    RpcClient();
    ~RpcClient();
    RpcClient(RpcClient&) = delete;
    RpcClient(RpcClient&&) = delete;
    /** 
     * initialize client
     */
    void Initialize();
    /** 
     * client's main procedure
     */
    int Main(int argc, char* argv[]);
    /** 
     * client send rpc request
     * Not reentrant now, consider to use task queue in the future
     */
    void SendRequest(const RpcMessage& Message);
    /** 
     * Bind a proxy to this
     */
    void Bind(RpcServiceProxy* ServiceProxy);

private:
    EventHandlerRouter* router;
    Poller* poller;
    ClientConnectionManager* ClientConnectionMgr;
    CallbacksHandler* CallbacksHdr;

public:
    std::atomic<bool> m_stop;
};
