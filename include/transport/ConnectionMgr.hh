#pragma once

#include <runtime/iomodel/EventHandler.hh>

class Poller;
class EventHandlerManager;

class ConnectionManager: public EventHandler
{
public:
    ConnectionManager(
        Poller* InPoller, 
        EventHandlerManager* InEventHandlerMgr,
        EventHandler* InReader
        );
    virtual ~ConnectionManager();

    /**
     * If one EPOLLIN triggered at listenfd, try set up a new tcp connection 
     */
    virtual void HandleReadEvent(int Fd) override;
    /**
     * If one EPOLLHUP triggered, we should close this connection
     */
    virtual void HandleCloseEvent(int Fd) override;
    /**
     * Listen wrapper
     */
    void Listen();
private:
    /**
     * Accept wrapper
     */
    void HandleAcceptEvent();
    /**
     * Add a new fd
     */
    void Add(int Fd);
    /**
     * Delete a fd
     */
    void Del(int Fd);

    Poller* _Poller;
    EventHandlerManager* EventHandlerMgr;
    EventHandler* Reader;
    int ListenFd;
};
