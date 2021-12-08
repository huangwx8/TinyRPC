#include <stdio.h>

#include <common/RpcServiceProxy.hh>
#include <common/RpcTypes.hh>
#include <client/RpcClient.hh>
#include <server/RpcServer.hh>


void RpcServiceProxy::Invoke(const RpcMessage& Context) 
{ 
    if (!Client)
    {
        printf("RpcServiceProxy::Invoke: Error! Illegal invocation of Invoke, please [BindRpcClient] first!\n");
        return;
    }
    Client->SendRequest(Context);
};

void RpcServiceProxy::Handle(const RpcMessage& Context) 
{
    throw "RpcServiceProxy::Handle is not implemented!\n";
};

void RpcServiceProxy::BindRpcClient(RpcClient* InClient)
{
    Client = InClient;
}

void RpcServiceProxy::BindRpcServer(RpcServer* InServer)
{
    InServer->RegisterService(this);
}

const char* RpcServiceProxy::GetServiceName()
{
    return ServiceName;
}
