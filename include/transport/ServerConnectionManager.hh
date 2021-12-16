#pragma once

#include <functional>

#include <runtime/handlemodel/EventHandler.hh>

class Poller;
class EventHandlerManager;

class ServerConnectionManager: public EventHandler
{
public:
    ServerConnectionManager(
        std::function<void(int, bool)>,
        std::function<void(int)>
    );

    virtual ~ServerConnectionManager();

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
    int Accept();

    // Lambda delegate function, to avoid include higher modules (RpcServer)
    std::function<void(int, bool)> OnPostOpenFd;
    std::function<void(int)> OnPreCloseFd;

    // Listen file descriptor
    int ListenFd;
};
