#pragma once

#include <common/RpcTypes.hh>

class RpcClient;
class RpcServer;

class RpcServiceProxy
{
public:
    RpcServiceProxy() = default;
    virtual ~RpcServiceProxy() = default;
    virtual void Invoke(const RpcMessage& Context);
    virtual void Handle(const RpcMessage& Context);
    void BindRpcClient(RpcClient* InClient);
    void BindRpcServer(RpcServer* InServer);
    const char* GetServiceName();
protected:
    const char* ServiceName = "None";
private:
    RpcClient* Client;
};
