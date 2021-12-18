#include <stdio.h>

#include <common/RpcServiceProxy.hh>
#include <common/RpcTypes.hh>
#if BUILD_CLIENT
#include <client/RpcClient.hh>
#endif

void RpcServiceProxy::Invoke(const RpcMessage& Context) 
{ 
    if (!RpcPortal)
    {
        printf("RpcServiceProxy::Invoke: Error! Illegal invocation of Invoke, please make sure RpcClient.Bind() is done!\n");
        return;
    }
#if BUILD_CLIENT
    RpcPortal->SendRequest(Context);
#endif
};

int RpcServiceProxy::Handle(const RpcMessage& Context) 
{
    throw "RpcServiceProxy::Handle is not implemented!\n";
};

const char* RpcServiceProxy::GetServiceName()
{
    return ServiceName;
}
