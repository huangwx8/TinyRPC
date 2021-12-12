#include <stdio.h>
#include <algorithm>

#include <runtime/handlemodel/EventHandlerManager.hh>

#define MAX_FILE_DESCRIPTORS 65536
#define MAX_EVENT_HANDLERS (MAX_FILE_DESCRIPTORS << 2)


static inline int GetEventId(int Fd, EventHandler::EventType Type)
{
    return (Fd << 2) | EventHandler::EventTypeToEventId(Type);
}

EventHandler* EventHandlerManager::CreateInstance()
{
    return new EventHandlerManager;
}

EventHandlerManager::EventHandlerManager():
    _list(new EventHandler *[MAX_EVENT_HANDLERS])
{
    std::fill(_list, _list + MAX_EVENT_HANDLERS, nullptr);
}

EventHandlerManager::~EventHandlerManager()
{
    delete[] _list;
}

void EventHandlerManager::HandleEvent(int Fd, EventType Type)
{
    //printf("EventHandlerManager::HandleEvent Fd = [%d], Type = [%d]\n", Fd, Type);
    if (Type == 0)
    {
        return;
    }
    if (EventHandler* Handler = _list[GetEventId(Fd, Type)])
    {
        Handler->HandleEvent(Fd, Type);
    }
    else 
    {
        printf("EventHandlerManager::HandleEvent: Warning! Get EventHandler for fd=[%d], Type=[%d] failed!\n", Fd, static_cast<int>(Type));
    }
}

void EventHandlerManager::AttachEventHandler(int Fd, EventType Type, EventHandler* Handler)
{
    if (Handler)
    {
        SetEventHandler(Fd, Type, Handler);
    }
}

void EventHandlerManager::DetachEventHandler(int Fd, EventType Type)
{
    SetEventHandler(Fd, Type, nullptr);
}

void EventHandlerManager::SetEventHandler(int Fd, EventType Type, EventHandler* Handler)
{
    if (Type & EventType::READ_EVENT)
    {
        _list[Fd << 2] = Handler;
    }
    if (Type & EventType::WRITE_EVENT)
    {
        _list[(Fd << 2) | 1] = Handler;
    }
    if (Type & EventType::CLOSE_EVENT)
    {
        _list[(Fd << 2) | 2] = Handler;
    }
}
