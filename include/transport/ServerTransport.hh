#pragma once

#include <string>
#include <functional>

#include <runtime/iomodel/Poller.hh>
#include <runtime/iomodel/Reactor.hh>
#include <runtime/handlemodel/EventHandler.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>

class Poller;
class EventHandlerManager;

class ServerTransport: public EventHandler
{
public:
    ServerTransport(EventHandler*, EventHandler*);

    virtual ~ServerTransport();

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
    void Listen(std::string ip, int port);

    Reactor& GetReactor() { return _reactor; }

private:
    /**
     * Accept wrapper
     */
    int Accept();

    // event handler for connected sockets
    EventHandler* _socket_reader;
    EventHandler* _socket_writer;

    // event handler center
    EventHandlerManager _event_handler_manager;

    // event model
    Reactor _reactor;

    // Listen file descriptor
    int _listenfd;
};
