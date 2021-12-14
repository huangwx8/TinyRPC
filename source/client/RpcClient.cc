#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include "sys/epoll.h"

#include <client/RpcClient.hh>

#include <common/RpcServiceProxy.hh>
#include <transport/ClientConnectionManager.hh>
#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>


void EventHandlerRouter::HandleEvent(int Fd, EventType Type)
{
    switch (Type)
    {
    case CLOSE_EVENT:
        Closehdr->HandleCloseEvent(Fd);
        break;
    case READ_EVENT:
        Readhdr->HandleReadEvent(Fd);
        break;
    case WRITE_EVENT:
        Writehdr->HandleWriteEvent(Fd);
        break;
    default:
        break;
    }
}

RpcClient::RpcClient():
    router(nullptr),
    poller(nullptr),
    ClientConnectionMgr(nullptr)
{

}

RpcClient::~RpcClient()
{
    if (router)
    {
        delete router;
        router = nullptr;
    }
    if (poller)
    {
        delete poller;
        poller = nullptr;
    }
    if (ClientConnectionMgr)
    {
        delete ClientConnectionMgr;
        ClientConnectionMgr = nullptr;
    }
}

void RpcClient::Initialize()
{
    // create poller
    poller = new Poller();
    // connection
    ClientConnectionMgr = new ClientConnectionManager();
    // create event handlers
    router = new EventHandlerRouter(nullptr, nullptr, nullptr);
}

int RpcClient::Main(int argc, char* argv[])
{
    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char * ip = argv[1];
    int port = atoi(argv[2]);

    // CltConnMgr建立与服务器的连接
    int Connfd = ClientConnectionMgr->Connect(ip, port);

    // 由CltConnMgr处理CLOSE事件
    router->Closehdr = ClientConnectionMgr;
    // 由CallbacksHdr处理READ事件
    // router->Readhdr = CallbacksHandler;
    // 由RequstSdr处理WRITE事件
    // router->Writehdr = RpcRequestSender;

    // 监听所有事件
    poller->AddEvent(Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);

    return 0;
}

void RpcClient::SendRequest(const RpcMessage& Message)
{
    ClientConnectionMgr->Send(Message);
}

void RpcClient::Bind(RpcServiceProxy* ServiceProxy)
{
    ServiceProxy->RpcPortal = this;
}
