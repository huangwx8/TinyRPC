#pragma once

// inner
#include <runtime/handlemodel/EventHandler.hh>

/**
 * 聚合多个EventHandler，并能针对到来的事件，选择该使用哪个Handler
 */
class EventHandlerManager: public EventHandler 
{
public:
    EventHandlerManager();
    virtual ~EventHandlerManager();

    virtual void HandleEvent(int Fd, EventType Type) override;

    void AttachEventHandler(int Fd, EventType Type, EventHandler* Handler);
    void DetachEventHandler(int Fd, EventType Type);

private:
    void SetEventHandler(int Fd, EventType Type, EventHandler* Handler);
    EventHandler** _list;
};
