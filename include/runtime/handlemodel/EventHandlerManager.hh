#pragma once

// inner
#include <runtime/handlemodel/EventHandler.hh>

/**
 * 中转站
 */
class EventHandlerRouter: public EventHandler 
{
public:
    EventHandlerRouter():
        Readhdl(nullptr),
        Writehdl(nullptr),
        Closehdl(nullptr)
    {}

    virtual ~EventHandlerRouter() {}

    virtual void HandleEvent(int Fd, EventType Type) override;

    EventHandler* Readhdl;
    EventHandler* Writehdl;
    EventHandler* Closehdl;
};

/**
 * 聚合多个EventHandler，并能针对到来的事件，选择该使用哪个Handler
 */
class EventHandlerManager: public EventHandler 
{
public:
    static EventHandler* CreateInstance();

    EventHandlerManager();
    virtual ~EventHandlerManager();

    virtual void HandleEvent(int Fd, EventType Type) override;

    void AttachEventHandler(int Fd, EventType Type, EventHandler* Handler);
    void DetachEventHandler(int Fd, EventType Type);

private:
    void SetEventHandler(int Fd, EventType Type, EventHandler* Handler);
    EventHandler** _list;
};
