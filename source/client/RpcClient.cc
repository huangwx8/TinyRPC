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
#include <transport/ClientTransport.hh>
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
        Readhdl(nullptr),
        Writehdl(nullptr),
        Closehdl(nullptr)
    {}

    virtual ~EventHandlerRouter() {}

    virtual void HandleEvent(int Fd, EventType Type) override;

    EventHandler* Readhdl;
    EventHandler* Writehdl;
    EventHandler* Closehdl;
};

void EventHandlerRouter::HandleEvent(int Fd, EventType Type)
{
    switch (Type)
    {
    case CLOSE_EVENT:
        Closehdl->HandleCloseEvent(Fd);
        break;
    case READ_EVENT:
        Readhdl->HandleReadEvent(Fd);
        break;
    case WRITE_EVENT:
        Writehdl->HandleWriteEvent(Fd);
        break;
    default:
        break;
    }
}

RpcClient::RpcClient():
    router(nullptr),
    poller(nullptr),
    Transport(nullptr),
    Callbackshdl(nullptr),
    m_stop(false)
{

}

RpcClient::~RpcClient()
{
    // Stop main()
    m_stop = true;
    // we cannot start destructor procedure before main() returns
    std::unique_lock<std::mutex> lock(m);
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
    if (Transport)
    {
        delete Transport;
        Transport = nullptr;
    }
    if (Callbackshdl)
    {
        delete Callbackshdl;
        Callbackshdl = nullptr;
    }
}

void RpcClient::Initialize()
{
    // create poller
    poller = new Poller(false);
    // connection
    Transport = new ClientTransport();
    // create event handlers
    router = new EventHandlerRouter();
    Callbackshdl = new CallbacksHandler();
}

int RpcClient::Main(int argc, char* argv[])
{
    std::unique_lock<std::mutex> lock(m);

    if (argc <= 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }
    const char * ip = argv[1];
    int port = atoi(argv[2]);

    // CltConnMgr建立与服务器的连接
    int Connfd = Transport->Connect(ip, port);

    // 由CltConnMgr处理CLOSE事件
    router->Closehdl = Transport;
    // 由CltConnMgr处理WRITE事件
    router->Writehdl = Transport;
    // 由Callbackshdl处理READ事件
    router->Readhdl = Callbackshdl;
    // 监听所有事件
    poller->AddEvent(Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);

    float timeout = 0.1f;

    while (!m_stop)
    {
        // wait, but not block, or this loop may dead
        auto&& tasks = poller->Dispatch(timeout, *router);
        // process
        for (auto&& task : tasks)
        {
            task();
        }
    }

    return 0;
}

void RpcClient::SendRequest(const RpcMessage& Message, std::function<void(int)> Callback)
{
    Transport->Send(Message);
    Callbackshdl->CallidCallbackMapping[Message.Callid] = Callback;
}

void RpcClient::Bind(RpcServiceProxy* ServiceProxy)
{
    ServiceProxy->RpcPortal = this;
}
