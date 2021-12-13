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

RpcClient::RpcClient():
    ClientConnectionMgr(nullptr),
    poller(nullptr)
{

}

RpcClient::~RpcClient()
{
    if (ClientConnectionMgr)
    {
        delete ClientConnectionMgr;
        ClientConnectionMgr = nullptr;
    }
    if (poller)
    {
        delete poller;
        poller = nullptr;
    }
}

void RpcClient::Initialize()
{
    // create event handlers
    EventHandlerMgr = new EventHandlerManager();
    // create poller
    poller = new Poller();
    // connection
    ClientConnectionMgr = new ClientConnectionManager(EventHandlerMgr);
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

    int Connfd = ClientConnectionMgr->Connect(ip, port);
    // todo: 为connfd添加read和write handler
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
