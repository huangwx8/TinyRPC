// std c
#include <cstdio>
#include <cstring>

// linux
#include <sys/socket.h>

#include <client/CallbacksHandler.hh>

#include <common/RpcTypes.hh>
#include <common/Guid.hh>
#include <common/Defines.hh>

static void ErrorCallback(int retval)
{
    throw "Uninitialized callback function, client may receive a bad rpc response packet.";
}

CallbacksHandler::CallbacksHandler():
    CallidCallbackMapping(MAX_REQUESTS, std::function<void(int)>(&ErrorCallback))
{
    
}

CallbacksHandler::~CallbacksHandler()
{

}

void CallbacksHandler::HandleReadEvent(int Fd)
{
    RpcResult res;
    // bzero receive buffer and receive
    bzero(&res, sizeof(RpcResult));
    int ret = recv(Fd, &res, sizeof(RpcResult), 0);
    if (ret == -1)
    {
        printf("CallbacksHandler::HandleReadEvent: receive failed.\n");
        return;
    }

    int callid = res.Callid, retval = res.RetVal;
    (CallidCallbackMapping[callid])(retval);
    CallidCallbackMapping[callid] = &ErrorCallback;

    Guid::RecycleGuid(callid);
}
