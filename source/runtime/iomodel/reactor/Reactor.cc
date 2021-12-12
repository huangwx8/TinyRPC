// self
#include <runtime/iomodel/reactor/Reactor.hh>

Reactor::Reactor(Poller* InPoller, EventHandler* InEventHandler):
    _Threadpool(4, 10000),
    _Poller(InPoller),
    MainHandler(InEventHandler)
{

}

Reactor::~Reactor()
{
    if (MainHandler)
    {
        delete MainHandler;
    }
}

void Reactor::Run()
{
    while (1) 
    {
        auto task = [this]()
        {
            this->_Poller->Dispatch(-1, *MainHandler);
        };
        _Threadpool.AddTask(task);
    }
}
