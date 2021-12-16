#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <transport/ClientConnectionManager.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>
#include <runtime/iomodel/reactor/Poller.hh>

static int ConnectTo(const char* ip, int port)
{
    struct sockaddr_in server_address;
    bzero(&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server_address.sin_addr);
    server_address.sin_port = htons(port);

    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(sockfd >= 0);

    if (connect(sockfd, (struct sockaddr*)&server_address, sizeof(server_address)) < 0)
    {
        printf("connection failed\n");
        close(sockfd);
    }

    return sockfd;
}

ClientConnectionManager::ClientConnectionManager():
    Connfd(-1)
{
   
}

ClientConnectionManager::~ClientConnectionManager()
{
    close(Connfd);
}

void ClientConnectionManager::HandleCloseEvent(int Fd)
{
    throw "Server close";
}

int ClientConnectionManager::Connect(const char* ip, int port)
{
    Connfd = ConnectTo(ip, port);
    if (Connfd < 0)
    {
        throw "Connect failed";
    }
    return Connfd;
}

void ClientConnectionManager::Send(const RpcMessage& Message)
{
    int ret = send(Connfd, &Message, sizeof(RpcMessage), 0);
    printf("%d\n",ret);
}
