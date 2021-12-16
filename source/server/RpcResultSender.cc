#include <sys/socket.h>

#include <server/RpcResultSender.hh>


RpcResultSender::RpcResultSender(std::function<void(int)> PostSendResult):
    OnFinishSendResult(PostSendResult)
{

}

void RpcResultSender::HandleWriteEvent(int Fd)
{
    int Connfd = Fd;
    int RetVal = 1;
    int ret = send(Connfd, &RetVal, sizeof(RetVal), 0);
    OnFinishSendResult(Fd);
}
