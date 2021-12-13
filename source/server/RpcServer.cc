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
    // reactor
    reactor = new Reactor(poller, dynamic_cast<EventHandler*>(EventHandlerMgr));
    // connections
    ServerConnectionMgr = new ServerConnectionManager(poller, EventHandlerMgr, RequestHandler);
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
