#pragma once

#include <functional>

// inner
#include <runtime/handlemodel/EventHandler.hh>

class RpcResultSender: public EventHandler
{
public:
    RpcResultSender(std::function<std::pair<int,int>(int)> GetResultPair, std::function<void(int)>);
    virtual ~RpcResultSender() = default;
    /**
     * If one EPOLLOUT triggered at a connfd, try prepare a response datagram and send to it
     */
    virtual void HandleWriteEvent(int Fd) override;

private:
    std::function<std::pair<int,int>(int)> GetData;
    std::function<void(int)> OnFinishSendResult;
};
