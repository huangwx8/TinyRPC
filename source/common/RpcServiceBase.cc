#include <stdio.h>

#include <common/RpcServiceBase.hh>
#include <common/RpcTypes.hh>
#include <common/Logger.hh>
#if BUILD_CLIENT
#include <client/RpcClient.hh>
#endif

// for users do not provide a explicit callback function
static void EmptyCallback(int RetVal) {}

RpcServiceBase::RpcServiceBase():
    Callback(&EmptyCallback)
{

}

void RpcServiceBase::Invoke(const RpcMessage& Context) 
{ 
    if (!RpcPortal)
    {
        log_dev("RpcServiceBase::Invoke: Error! Illegal invocation of Invoke, please make sure RpcClient.Bind() is done!\n");
        return;
    }
#if BUILD_CLIENT
    RpcPortal->SendRequest(Context, Callback);
#endif
};

int RpcServiceBase::Handle(const RpcMessage& Context) 
{
    log_err("RpcServiceBase::Handle is not implemented!\n");
    exit(1);
};

const char* RpcServiceBase::GetServiceName()
{
    return ServiceName;
}

void RpcServiceBase::SetCallback(std::function<void(int)> Func)
{
    Callback = Func;
}
