#include <stdio.h>

#include <common/RpcServiceBase.hh>
#include <common/RpcTypes.hh>
#include <common/Logger.hh>

// for users do not provide a explicit callback function
static void EmptyCallback(int retval) {}

RpcServiceBase::RpcServiceBase()
#ifdef BUILD_CLIENT
    :Callback(&EmptyCallback)
#endif
{

}

const char* RpcServiceBase::GetServiceName()
{
    return ServiceName;
}

#ifdef BUILD_SERVER
int RpcServiceBase::Handle(const RpcMessage& Context) 
{
    log_err("RpcServiceBase::Handle is not implemented!\n");
    exit(1);
};
#endif

#ifdef BUILD_CLIENT
void RpcServiceBase::Invoke(const RpcMessage& Context) 
{ 
    SendChannel(Context, Callback);
};

void RpcServiceBase::SetCallback(const FCallback& Func)
{
    Callback = Func;
}

void RpcServiceBase::SetSendChannel(const FSendChannel& Func)
{
    SendChannel = Func;
}
#endif
