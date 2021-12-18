#pragma once

#include <functional>

#include <common/RpcTypes.hh>

class RpcClient;

class RpcServiceProxy
{
    friend RpcClient;
public:
    RpcServiceProxy();
    virtual ~RpcServiceProxy() = default;
    virtual void Invoke(const RpcMessage& Context);
    virtual int Handle(const RpcMessage& Context);
    const char* GetServiceName();
    void SetCallback(std::function<void(int)>);
protected:
    const char* ServiceName = "None";
private:
    RpcClient* RpcPortal;
    std::function<void(int)> Callback;
};
