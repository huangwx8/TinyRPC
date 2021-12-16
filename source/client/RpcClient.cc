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
#include <client/CallbacksHandler.hh>


/**
 * 中转站
 */
class EventHandlerRouter: public EventHandler 
{
public:
    EventHandlerRouter():
        Readhdr(nullptr),
        Writehdr(nullptr),
        Closehdr(nullptr)
    {}

    virtual ~EventHandlerRouter() {}

    virtual void HandleEvent(int Fd, EventType Type) override;

    EventHandler* Readhdr;
    EventHandler* Writehdr;
    EventHandler* Closehdr;
};

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
    ClientConnectionMgr(nullptr),
    CallbacksHdr(nullptr),
    m_stop(false)
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
    if (CallbacksHdr)
    {
        delete CallbacksHdr;
        CallbacksHdr = nullptr;
    }
}

void RpcClient::Initialize()
{
    // create poller
    poller = new Poller();
    // connection
    ClientConnectionMgr = new ClientConnectionManager();
    // create event handlers
    router = new EventHandlerRouter();
    CallbacksHdr = new CallbacksHandler();
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
    router->Readhdr = CallbacksHdr;;
    // 由RequstSdr处理WRITE事件
    // router->Writehdr = RpcRequestSender;
    // 监听所有事件
    poller->AddEvent(Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);

    while (!m_stop)
    {
        // wait, but not block, or this loop may dead
        auto&& tasks = poller->Dispatch(1, *router);
        // process
        for (auto&& task : tasks)
        {
            task();
        }
    }

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
