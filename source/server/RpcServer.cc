// std
#include <functional>

// linux
#include <sys/epoll.h>

// inner
#include <server/RpcServer.hh>
#include <server/RpcRequestHandler.hh>

#include <common/RpcServiceProxy.hh>

#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/iomodel/reactor/Reactor.hh>
#include <runtime/handlemodel/EventHandler.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>
#include <runtime/threadmodel/ThreadPool.hh>

#include <transport/ServerConnectionManager.hh>


RpcServer::RpcServer():
    RequestHandler(nullptr),
    EventHandlerMgr(nullptr),
    poller(nullptr),
    reactor(nullptr),
    ServerConnectionMgr(nullptr)
{

}

RpcServer::~RpcServer()
{
    if (RequestHandler)
    {
        delete RequestHandler;
        RequestHandler = nullptr;
    }
    if (EventHandlerMgr)
    {
        delete EventHandlerMgr;
        EventHandlerMgr = nullptr;
    }
    if (poller)
    {
        delete poller;
        poller = nullptr;
    }
    if (reactor)
    {
        delete reactor;
        reactor = nullptr;
    }
    if (ServerConnectionMgr)
    {
        delete ServerConnectionMgr;
        ServerConnectionMgr = nullptr;
    }
}

void RpcServer::Initialize()
{
    // create event handlers
    RequestHandler = new RpcRequestHandler();
    EventHandlerMgr = new EventHandlerManager();
    // poller
    poller = new Poller();
    // connections
    std::function<void(int, bool)> RegisterEvent = [this](int Fd, bool bIsListen) {
        // 所有关闭连接的时间由ConnMgr处理
        EventHandlerMgr->AttachEventHandler(Fd, EventHandler::CLOSE_EVENT, this->ServerConnectionMgr);
        // 如果是listenfd，则由ConnMgr处理ReadEvent，否则转交对应的请求处理类或响应发送类
        if (bIsListen)
        {
            EventHandlerMgr->AttachEventHandler(Fd, EventHandler::READ_EVENT, this->ServerConnectionMgr);
        }
        else 
        {
            EventHandlerMgr->AttachEventHandler(Fd, EventHandler::READ_EVENT, RequestHandler);
            // EventHandlerMgr->AttachEventHandler(Fd, EventHandler::Write_EVENT, ResultSender);
        }
        poller->AddEvent(Fd, EPOLLIN | EPOLLERR | EPOLLRDHUP);
    };
    std::function<void(int)> UnregisterEvent = [this](int Fd) {
        EventHandlerMgr->DetachEventHandler(Fd, static_cast<EventHandler::EventType>(
            EventHandler::READ_EVENT | EventHandler::WRITE_EVENT | EventHandler::CLOSE_EVENT
            )
        );
        poller->DelEvent(Fd, 0);
    };
    ServerConnectionMgr = new ServerConnectionManager(poller, EventHandlerMgr, RequestHandler);
    // reactor
    reactor = new Reactor(poller, dynamic_cast<EventHandler*>(EventHandlerMgr));
}

void RpcServer::RegisterService(RpcServiceProxy* Service)
{
    RequestHandler->AddProxy(Service);
}

int RpcServer::Main(int argc, char* argv[])
{
    ServerConnectionMgr->Listen();
    reactor->Run();
    return 0;
}
