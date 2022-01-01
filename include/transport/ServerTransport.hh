#pragma once

#include <string>
#include <functional>

#include <runtime/handlemodel/EventHandler.hh>

class Poller;
class EventHandlerManager;

class ServerTransport: public EventHandler
{
public:
    ServerTransport(
        std::function<void(int, bool)>,
        std::function<void(int)>
    );

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

    // Todo: Kill dead connections
    /**
     * update last active time, register a new timer task
     */
    // void OnConnectionActivated(int Fd);
    // std::array<int, MAX_FILE_DESCRIPTORS> ConnectionLastActiveTime;
};
