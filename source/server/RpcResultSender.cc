#include <stdio.h>
#include <sys/socket.h>

#include <server/RpcResultSender.hh>
#include <common/RpcTypes.hh>


RpcResultSender::RpcResultSender(
    std::function<RpcResult(int)> GetResultPair,
    std::function<void(int)> PostSendResult
):
    GetData(GetResultPair),
    OnFinishSendResult(PostSendResult)
{

}

void RpcResultSender::HandleWriteEvent(int Fd)
{
    printf("RpcResultSender::HandleWriteEvent: Fd = [%d]\n", Fd);
    
    int Connfd = Fd;
    RpcResult Result = GetData(Fd);
    int ret = send(Connfd, &Result, sizeof(RpcResult), 0);
    OnFinishSendResult(Fd);
}
