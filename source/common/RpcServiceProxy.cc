#include <stdio.h>

#include <common/RpcServiceProxy.hh>
#include <common/RpcTypes.hh>
#include <common/Logger.hh>
#if BUILD_CLIENT
#include <client/RpcClient.hh>
#endif

// for users do not provide a explicit callback function
static void EmptyCallback(int RetVal) {}

RpcServiceProxy::RpcServiceProxy():
    Callback(&EmptyCallback)
{

}

void RpcServiceProxy::Invoke(const RpcMessage& Context) 
{ 
    if (!RpcPortal)
    {
        log_dev("RpcServiceProxy::Invoke: Error! Illegal invocation of Invoke, please make sure RpcClient.Bind() is done!\n");
        return;
    }
#if BUILD_CLIENT
    RpcPortal->SendRequest(Context, Callback);
#endif
};

int RpcServiceProxy::Handle(const RpcMessage& Context) 
{
    log_err("RpcServiceProxy::Handle is not implemented!\n");
    exit(1);
};

const char* RpcServiceProxy::GetServiceName()
{
    return ServiceName;
}

void RpcServiceProxy::SetCallback(std::function<void(int)> Func)
{
    Callback = Func;
}
