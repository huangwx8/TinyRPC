#pragma once

#include <vector>
#include <functional>

#include <runtime/handlemodel/EventHandler.hh>

class RpcClient;

class CallbacksHandler: public EventHandler
{
    friend RpcClient;
public:
    CallbacksHandler();
    ~CallbacksHandler();
    /**
     * When READ event triggered at a connected fd
     * it should read rpc result and execute corresponding callback function
     */
    virtual void HandleReadEvent(int Fd) override;

    void Register(int, std::function<void(int)>);

private:
    std::vector<std::function<void(int)>> CallidCallbackMapping;
};