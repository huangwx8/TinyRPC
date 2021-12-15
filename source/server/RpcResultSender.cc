#include <sys/socket.h>

#include <server/RpcResultSender.hh>


void RpcResultSender::HandleWriteEvent(int Fd)
{
    int Connfd = Fd;
    int RetVal = 1;
    int ret = send(Connfd, &RetVal, sizeof(RetVal), 0);
}
