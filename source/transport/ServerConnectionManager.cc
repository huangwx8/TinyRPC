// std c/c++
#include <assert.h>
#include <stdio.h>
#include <string.h>

// linux
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// inner
#include <transport/ServerConnectionManager.hh>
#include <runtime/handlemodel/EventHandler.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>
#include <runtime/iomodel/reactor/Poller.hh>

// c-style creator
static int CreateListenFd(const char* ip, int port)
{
    int ret = 0;
    // net data
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &address.sin_addr);
    address.sin_port = htons(port);
    // create socket
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);
    // bind
    ret = bind(listenfd, (struct sockaddr*) &address, sizeof(address));
    assert(ret != -1);
    // listen
    ret = listen(listenfd, 5);
    assert(ret != -1);

    return listenfd;
}

ServerConnectionManager::ServerConnectionManager(
    Poller* InPoller, 
    EventHandlerManager* InEventHandlerMgr,
    EventHandler* InReader
):
    _Poller(InPoller),
    EventHandlerMgr(InEventHandlerMgr),
    Reader(InReader),
    ListenFd(-1)
{
   
}

ServerConnectionManager::~ServerConnectionManager()
{
    if (ListenFd >= 0)
    {
        Del(ListenFd);
    }
}

void ServerConnectionManager::Listen()
{
    ListenFd = CreateListenFd("localhost", 8888);
    if (ListenFd >= 0)
    {
        Add(ListenFd);
        printf("ServerConnectionManager::Listen: Start listening at fd [%d]\n", ListenFd);
    }
    else 
    {
        throw "Listen failed";
    }
}

void ServerConnectionManager::HandleReadEvent(int Fd)
{
    HandleAcceptEvent();
}

void ServerConnectionManager::HandleCloseEvent(int Fd)
{
    Del(Fd);
}

void ServerConnectionManager::HandleAcceptEvent()
{
    // accept a new tcp connection request
    struct sockaddr_in ClientAddress;
    socklen_t ClientAddrLength = sizeof(ClientAddress);
    int Connfd = accept(ListenFd, (struct sockaddr*)&ClientAddress, &ClientAddrLength);
    if (Connfd < 0)
    {
        printf("ServerConnectionManager::HandleReadEvent: Accept failure, errno is %d\n", errno);
        return;
    }
    printf("ServerConnectionManager::HandleAcceptEvent: Create a connection at fd [%d]\n", Connfd);
    Add(Connfd);
}

void ServerConnectionManager::Add(int Fd)
{
    EventHandlerMgr->AttachEventHandler(Fd, EventHandler::CLOSE_EVENT, this);
    // 如果是listenfd，则由this处理ReadEvent，否则转交Reader
    if (ListenFd == Fd)
    {
        EventHandlerMgr->AttachEventHandler(Fd, EventHandler::READ_EVENT, this);
    }
    else 
    {
        EventHandlerMgr->AttachEventHandler(Fd, EventHandler::READ_EVENT, Reader);
    }
    _Poller->AddEvent(Fd, EPOLLIN | EPOLLERR | EPOLLRDHUP);
}

void ServerConnectionManager::Del(int Fd)
{
    printf("ServerConnectionManager::HandleCloseEvent: close fd [%d]\n", Fd);
    EventHandlerMgr->DetachEventHandler(Fd, static_cast<EventHandler::EventType>(EventHandler::READ_EVENT | EventHandler::WRITE_EVENT | EventHandler::CLOSE_EVENT));
    _Poller->DelEvent(Fd, 0);
    close(Fd);
}
