// inner
#include <server/RpcServer.hh>
#include <server/RpcRequestHandler.hh>

#include <common/RpcServiceProxy.hh>

#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/iomodel/reactor/Reactor.hh>
#include <runtime/iomodel/EventHandler.hh>
#include <runtime/iomodel/EventHandlerManager.hh>

#include <transport/ConnectionMgr.hh>


RpcServer::RpcServer():
    _Poller(nullptr),
    EventHandlerMgr(nullptr),
    ServerConnectionManager(nullptr),
    RequestHandler(nullptr),
    _Reactor(nullptr)
{

}

RpcServer::~RpcServer()
{
    if (_Poller)
    {
        delete _Poller;
        _Poller = nullptr;
    }
    if (EventHandlerMgr)
    {
        delete EventHandlerMgr;
        EventHandlerMgr = nullptr;
    }
    if (ServerConnectionManager)
    {
        delete ServerConnectionManager;
        ServerConnectionManager = nullptr;
    }
    if (RequestHandler)
    {
        delete RequestHandler;
        RequestHandler = nullptr;
    }
    if (_Reactor)
    {
        delete _Reactor;
        _Reactor = nullptr;
    }
}

void RpcServer::Initialize()
{
    // create poller
    _Poller = new Poller();
    // create event handlers
    EventHandlerMgr = new EventHandlerManager();
    RequestHandler = new RpcRequestHandler();
    ServerConnectionManager = new ConnectionManager(_Poller, EventHandlerMgr, RequestHandler);
    // reactor
    _Reactor = new Reactor(Reactor::Options{.id = 0, .poller = _Poller, .handler = EventHandlerMgr});
}

void RpcServer::RegisterService(RpcServiceProxy* Service)
{
    RequestHandler->AddProxy(Service);
}

int RpcServer::Main(int argc, char* argv[])
{
    ServerConnectionManager->Listen();
    _Reactor->Run();
}
