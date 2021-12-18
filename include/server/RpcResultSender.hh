#pragma once

#include <functional>

// inner
#include <runtime/handlemodel/EventHandler.hh>

#include <common/RpcTypes.hh>

class RpcResultSender: public EventHandler
{
public:
    RpcResultSender(std::function<RpcResult(int)> GetResultPair, std::function<void(int)>);
    virtual ~RpcResultSender() = default;
    /**
     * If one EPOLLOUT triggered at a connfd, try prepare a response datagram and send to it
     */
    virtual void HandleWriteEvent(int Fd) override;

private:
    std::function<RpcResult(int)> GetData;
    std::function<void(int)> OnFinishSendResult;
};
