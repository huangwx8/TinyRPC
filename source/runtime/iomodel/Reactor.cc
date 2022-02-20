// self
#include <runtime/iomodel/Reactor.hh>

Reactor::Reactor(EventHandler* event_handler):
    _thread_pool(4, 10000),
    _event_handler(event_handler),
    _poller(true)
{

}

Reactor::~Reactor()
{
}

void Reactor::Run()
{
    while (1) 
    {
        auto tasks = _poller.Dispatch(-1, *_event_handler);
        for (auto& task : tasks)
        {
            _thread_pool.AddTask(std::move(task));
        }
    }
}
