// std c/c++
#include <assert.h>
#include <stdio.h>
#include <string.h>

// unix
#include <sys/socket.h>

// inner
#include <server/RpcEventHandler.hh>
#include <common/Logger.hh>
#include <common/RpcUtils.hh>
#include <common/RpcTypes.hh>
#include <common/RpcServiceBase.hh>

RpcResult ReturnValuePipe::Read(int fd) 
{
    assert(!_return_values[fd].empty());
    // 从fd对应的等待队列中拿一个元素
    auto&& result = _return_values[fd].front();
    _return_values[fd].pop();
    return result;
}

void ReturnValuePipe::Write(int fd, RpcResult result) 
{
    _return_values[fd].push(result);
}

RpcRequestHandler::RpcRequestHandler(ReturnValuePipe* pipe, FileDescriptorEventDelegate* delegate): _pipe(pipe), _finished(delegate) {}

void RpcRequestHandler::HandleReadEvent(int connfd)
{
    RpcMessage Message;
    int TaskRetVal = -1;
    
    // bzero receive buffer and receive
    bzero(&Message, sizeof(RpcMessage));
    int ret = recv(connfd, &Message, sizeof(RpcMessage), 0);
    if (ret < 0)
    {
        // bad read
        if (errno != EAGAIN)
        {
            log_err("RpcRequestHandler::HandleReadEvent: Bad recv\n");
            exit(1);
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
        log_dev("RpcRequestHandler::HandleReadEvent: get rpc request from %d\n", connfd);
        std::string RpcName = Message.RpcName;
        if (_dictionary.find(RpcName) != _dictionary.end())
        {
            RpcServiceBase* Service = _dictionary[RpcName];
            TaskRetVal = Service->Handle(Message);
        }
        else 
        {
            log_dev("RpcRequestHandler::HandleReadEvent: Warning! Service [%s] not found\n", Message.RpcName);
        }
    }
    
    _pipe->Write(connfd, {Message.Callid, TaskRetVal});
    _finished->FileDescriptorEventDone(connfd);
}

void RpcRequestHandler::AddProxy(RpcServiceBase* Service)
{
    std::string ServiceName = Service->GetServiceName();
    if (_dictionary.find(ServiceName) == _dictionary.end())
    {
        _dictionary.insert(std::make_pair(ServiceName, Service));
    }
}

RpcResultSender::RpcResultSender(ReturnValuePipe* pipe, FileDescriptorEventDelegate* delegate): _pipe(pipe), _finished(delegate) {}

void RpcResultSender::HandleWriteEvent(int connfd)
{
    log_dev("RpcResultSender::HandleWriteEvent: fd = [%d]\n", connfd);
    
    RpcResult Result = _pipe->Read(connfd);
    int ret = send(connfd, &Result, sizeof(RpcResult), 0);
    _finished->FileDescriptorEventDone(connfd);
}
