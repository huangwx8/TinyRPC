#include <sys/socket.h>

#include <server/RpcResultSender.hh>


RpcResultSender::RpcResultSender(
    std::function<std::pair<int,int>(int)> GetResultPair,
    std::function<void(int)> PostSendResult
):
    GetData(GetResultPair),
    OnFinishSendResult(PostSendResult)
{

}

void RpcResultSender::HandleWriteEvent(int Fd)
{
    int Connfd = Fd;
    std::pair<int,int> Result = GetData(Fd);
    int Callid = Result.first, RetVal = Result.second;
    int ret = send(Connfd, &RetVal, sizeof(RetVal), 0);
    OnFinishSendResult(Fd);
}
