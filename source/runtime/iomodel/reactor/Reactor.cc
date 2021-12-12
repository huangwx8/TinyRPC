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
}

void Reactor::Run()
{
    while (1) 
    {
        auto tasks = _Poller->Dispatch(-1, *MainHandler);
        for (auto& task : tasks)
        {
            _Threadpool.AddTask(std::move(task));
        }
    }
}
