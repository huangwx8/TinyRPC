#include <stdio.h>

#include <common/RpcServiceProxy.hh>
#include <common/RpcTypes.hh>
#include <client/RpcClient.hh>


void RpcServiceProxy::Invoke(const RpcMessage& Context) 
{ 
    if (!RpcPortal)
    {
        printf("RpcServiceProxy::Invoke: Error! Illegal invocation of Invoke, please make sure RpcClient.Bind() is done!\n");
        return;
    }
    RpcPortal->SendRequest(Context);
};

void RpcServiceProxy::Handle(const RpcMessage& Context) 
{
    throw "RpcServiceProxy::Handle is not implemented!\n";
};

const char* RpcServiceProxy::GetServiceName()
{
    return ServiceName;
}
