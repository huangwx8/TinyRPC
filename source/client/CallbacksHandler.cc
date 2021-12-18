// std c
#include <cstdio>
#include <cstring>

// linux
#include <sys/socket.h>

#include <client/CallbacksHandler.hh>
#include <common/RpcTypes.hh>
#include <common/Guid.hh>

void CallbacksHandler::HandleReadEvent(int Fd)
{
    printf("CallbacksHandler::HandleReadEvent Fd = [%d]\n", Fd);

    RpcResult res;
    // bzero receive buffer and receive
    bzero(&res, sizeof(RpcResult));
    int ret = recv(Fd, &res, sizeof(RpcResult), 0);

    if (ret == -1)
    {
        printf("CallbacksHandler::HandleReadEvent: receive failed.\n");
    }
    
    printf("CallbacksHandler::HandleReadEvent Callid = [%d], Return value = [%d]\n", res.Callid, res.RetVal);

    Guid::RecycleGuid(res.Callid);
}
