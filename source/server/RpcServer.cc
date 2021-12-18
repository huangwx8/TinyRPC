// std
#include <functional>
#include <assert.h>

// linux
#include <sys/epoll.h>

// inner
#include <server/RpcServer.hh>
#include <server/RpcRequestHandler.hh>
#include <server/RpcResultSender.hh>

#include <common/RpcServiceProxy.hh>

#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/iomodel/reactor/Reactor.hh>
#include <runtime/handlemodel/EventHandler.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>
#include <runtime/threadmodel/ThreadPool.hh>

#include <transport/ServerConnectionManager.hh>


RpcServer::RpcServer():
    RequestHandler(nullptr),
    ResultSender(nullptr),
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
    if (ResultSender)
    {
        delete ResultSender;
        ResultSender = nullptr;
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
    // in handler
    std::function<void(int, int, int)> PostHandleRequest = [this](int Fd, int Callid, int RetVal) {
        // 注册返回值列表
        PendingResults[Fd].push({Callid, RetVal});
        // Epoll modify，如果缓冲区没有待处理的返回值，则关闭EPOLLOUT；
        // 同时，重设EPOLLONESHOT，如果此时有事件待处理，这将重新触发事件
        ResetOneshot(Fd);
    };
    RequestHandler = new RpcRequestHandler(PostHandleRequest);
    // out handler
    std::function<RpcResult(int)> GetResultPair = [this](int Fd) -> RpcResult {
        assert(!PendingResults[Fd].empty());
        // 从Fd对应的等待队列中拿一个元素
        auto&& pack = PendingResults[Fd].front();
        PendingResults[Fd].pop();
        return pack;
    };
    std::function<void(int)> PostSendResult = [this](int Fd) {
        ResetOneshot(Fd);
    };
    ResultSender = new RpcResultSender(GetResultPair, PostSendResult);
    // main handler
    EventHandlerMgr = new EventHandlerManager();

    // io multiplexing poller
    poller = new Poller(true);

    // connections
    std::function<void(int, bool)> RegisterEvent = [this](int Fd, bool bIsListen) {
        // 所有关闭连接的时间由ConnMgr处理
        EventHandlerMgr->AttachEventHandler(Fd, EventHandler::CLOSE_EVENT, this->ServerConnectionMgr);
        // 如果是listenfd，则由ConnMgr处理ReadEvent，否则转交对应的请求处理类或响应发送类
        if (bIsListen)
        {
            EventHandlerMgr->AttachEventHandler(Fd, EventHandler::READ_EVENT, this->ServerConnectionMgr);
            poller->AddEvent(Fd, EPOLLIN | EPOLLERR | EPOLLRDHUP);
        }
        else 
        {
            EventHandlerMgr->AttachEventHandler(Fd, EventHandler::READ_EVENT, RequestHandler);
            EventHandlerMgr->AttachEventHandler(Fd, EventHandler::WRITE_EVENT, ResultSender);
            poller->AddEvent(Fd, EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLONESHOT);
        }
    };
    std::function<void(int)> UnregisterEvent = [this](int Fd) {
        EventHandlerMgr->DetachEventHandler(Fd, static_cast<EventHandler::EventType>(
            EventHandler::READ_EVENT | EventHandler::WRITE_EVENT | EventHandler::CLOSE_EVENT
            )
        );
        poller->DelEvent(Fd, 0);
    };
    ServerConnectionMgr = new ServerConnectionManager(RegisterEvent, UnregisterEvent);

    // reactor event handle model, is the procedure of main thread
    reactor = new Reactor(poller, dynamic_cast<EventHandler*>(EventHandlerMgr));
}

void RpcServer::RegisterService(RpcServiceProxy* Service)
{
    RequestHandler->AddProxy(Service);
}

int RpcServer::Main(int argc, char* argv[])
{
    // Listen to clients' connect()
    ServerConnectionMgr->Listen();
    // poller select several events, deliver them to main handler, and then repeat
    reactor->Run();
    return 0;
}

void RpcServer::ResetOneshot(int Fd)
{
    if (!poller) return;

    if (PendingResults[Fd].empty())
    {
        poller->ModEvent(Fd, EPOLLIN | EPOLLERR | EPOLLRDHUP | EPOLLONESHOT);
    }
    else 
    {
        poller->ModEvent(Fd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP | EPOLLONESHOT);
    }
}
