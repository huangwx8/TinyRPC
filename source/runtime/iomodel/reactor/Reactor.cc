// inner
#include <runtime/iomodel/reactor/Reactor.hh>
#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/iomodel/EventHandler.hh>

Reactor::Reactor(const Options& options):
    _Poller(options.poller),
    Handler(options.handler)
{
    
}

Reactor::~Reactor()
{
    
}

void Reactor::Run()
{
    while (1) 
    {
        _Poller->Dispatch(-1, *Handler);
    }
}
