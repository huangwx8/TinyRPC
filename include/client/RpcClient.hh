#pragma once

#include <atomic>
#include <mutex>
#include <functional>
#include <condition_variable>

#include <common/RpcTypes.hh>

class Poller;
class RpcServiceBase;
class EventHandlerRouter;
class ClientTransport;
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
    void SendRequest(const RpcMessage& Message, std::function<void(int)>);
    /** 
     * Bind a proxy to this
     */
    void Bind(RpcServiceBase* ServiceProxy);
    /** 
     * Get a rpc client
     */
    static RpcClient& GetRpcClient(int argc, char* argv[]);

private:
    EventHandlerRouter* router;
    Poller* poller;
    ClientTransport* Transport;
    CallbacksHandler* Callbackshdl;

    // concurrency
    mutable std::mutex m;
    mutable std::mutex m2;
    std::condition_variable c;
    std::atomic<bool> initialized;
    std::atomic<bool> m_stop;
};
