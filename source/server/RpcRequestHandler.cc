// std c/c++
#include <stdio.h>
#include <string.h>

// linux
#include <sys/socket.h>

// inner
#include <server/RpcRequestHandler.hh>
#include <common/Logger.hh>
#include <common/RpcUtils.hh>
#include <common/RpcTypes.hh>
#include <common/RpcServiceBase.hh>


RpcRequestHandler::RpcRequestHandler(std::function<void(int, int, int)> PostHandleRequest):
    OnFinishTask(PostHandleRequest)
{
    
}

void RpcRequestHandler::HandleReadEvent(int Fd)
{
    int Connfd = Fd;
    RpcMessage Message;
    int TaskRetVal = -1;
    // bzero receive buffer and receive
    bzero(&Message, sizeof(RpcMessage));
    int ret = recv(Connfd, &Message, sizeof(RpcMessage), 0);
    if (ret < 0)
    {
        // bad read
        if (errno != EAGAIN)
        {
            log_dev("RpcRequestHandler::HandleReadEvent: Bad recv\n");
            return;
        }
    }
    // every bit has been read
    else if (ret == 0)
    {
    }
    // successfully got some bits, try run corresponding rpc service
    else
    {
        log_dev("RpcRequestHandler::HandleReadEvent: get rpc request from %d\n", Connfd);
        std::string RpcName = Message.RpcName;
        if (RpcServiceDict.find(RpcName) != RpcServiceDict.end())
        {
            RpcServiceBase* Service = RpcServiceDict[RpcName];
            TaskRetVal = Service->Handle(Message);
        }
        else
        {
            log_dev("RpcRequestHandler::HandleReadEvent: Warning! Service [%s] not found\n", Message.RpcName);
        }
    }
    OnFinishTask(Fd, Message.Callid, TaskRetVal);
}

void RpcRequestHandler::AddProxy(RpcServiceBase* Service)
{
    std::string ServiceName = Service->GetServiceName();
    if (RpcServiceDict.find(ServiceName) == RpcServiceDict.end())
    {
        RpcServiceDict.insert(std::make_pair(ServiceName, Service));
    }
}
