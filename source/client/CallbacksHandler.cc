// std c
#include <cstdio>
#include <cstring>

// linux
#include <sys/socket.h>

#include <client/CallbacksHandler.hh>

#include <common/RpcTypes.hh>
#include <common/Guid.hh>
#include <common/Defines.hh>
#include <common/Logger.hh>

static void ErrorCallback(int retval)
{
    log_err("Uninitialized callback function, client may receive a bad rpc response packet.");
    exit(1);
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
        log_dev("CallbacksHandler::HandleReadEvent: receive failed.\n");
        return;
    }

    int seqno = res.seqno, retval = res.retval;
    (CallidCallbackMapping[seqno])(retval);
    CallidCallbackMapping[seqno] = &ErrorCallback;

    Guid::RecycleGuid(seqno);
}
