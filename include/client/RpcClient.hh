#pragma once

#include <atomic>
#include <mutex>
#include <functional>
#include <string>
#include <condition_variable>

#include <common/RpcTypes.hh>

#include <runtime/handlemodel/EventHandlerManager.hh>
#include <runtime/iomodel/Poller.hh>

#include <transport/ClientTransport.hh>

#include <client/CallbacksHandler.hh>

class Poller;
class RpcServiceBase;
class EventHandlerRouter;
class ClientTransport;
class CallbacksHandler;

class RpcClient
{
public:
    struct Options
    {
        std::string svr_ip;
        int svr_port;
        std::string log_path;
    };

public:
    RpcClient(Options);
    ~RpcClient();
    RpcClient(RpcClient&) = delete;
    RpcClient(RpcClient&&) = delete;
    /** 
     * Construct and initialize
     */
    static std::shared_ptr<RpcClient> GetRpcClient(Options options);
    /**
     * Proxy creator
     */
    template<typename T>
    std::shared_ptr<T> GetProxy()
    {
        std::shared_ptr<T> proxy_ptr = std::make_shared<T>();
        proxy_ptr->SetSendChannel(
            [this](const RpcMessage& m, std::function<void(int)> c)
            {
                SendRequest(m, c);
            }
        );
        return proxy_ptr;
    }

private:
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
    
    Options _options;

    // worker
    ClientTransport _transport;
    EventHandlerRouter _router;
    CallbacksHandler _callback_handler;

    // concurrency
    mutable std::mutex m{};
    std::atomic<bool> _initialized;
    std::atomic<bool> m_stop;
};
