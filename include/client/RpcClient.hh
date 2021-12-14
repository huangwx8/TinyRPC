#pragma once

#include <common/RpcTypes.hh>

class Poller;
class RpcServiceProxy;
class EventHandlerManager;
class ClientConnectionManager;

/**
 * 中转站
 */
class EventHandlerRouter: public EventHandler 
{
public:
    EventHandlerRouter():
        Readhdr(nullptr),
        Writehdr(nullptr),
        Closehdr(nullptr)
    {}

    virtual ~EventHandlerRouter() {}

    virtual void HandleEvent(int Fd, EventType Type) override;

    EventHandler* Readhdr;
    EventHandler* Writehdr;
    EventHandler* Closehdr;
};

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
};
