#pragma once

#include <common/RpcTypes.hh>

class RpcClient;

class RpcServiceProxy
{
    friend RpcClient;
public:
    RpcServiceProxy() = default;
    virtual ~RpcServiceProxy() = default;
    virtual void Invoke(const RpcMessage& Context);
    virtual int Handle(const RpcMessage& Context);
    const char* GetServiceName();
protected:
    const char* ServiceName = "None";
private:
    RpcClient* RpcPortal;
};
