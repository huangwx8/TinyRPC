#pragma once

// std
#include <string>
#include <queue>
#include <array>
#include <unordered_map>
#include <functional>

// inner
#include <common/Defines.hh>
#include <common/RpcTypes.hh>
#include <runtime/handlemodel/EventHandler.hh>


class FileDescriptorEventDelegate 
{
public:
    virtual void FileDescriptorEventDone(int fd) = 0;
};

class ReturnValuePipe
{
public:
    RpcResult Read(int fd);
    void Write(int fd, RpcResult result);
    bool Empty(int fd) { return _return_values[fd].empty(); }
private:
    std::array<std::queue<RpcResult>, MAX_FILE_DESCRIPTORS> _return_values;
};

class RpcServiceBase;

class RpcRequestHandler: public EventHandler
{
public:
    RpcRequestHandler(ReturnValuePipe*, FileDescriptorEventDelegate*);
    virtual ~RpcRequestHandler() = default;
    /**
     * If one EPOLLIN triggered at a connfd, try find the matching rpc implematation to process
     */
    virtual void HandleReadEvent(int fd) override;
    /**
     * Add a rpc service implementation object into hash map, it can be found in next HandleReadEvent
     */
    void AddProxy(RpcServiceBase* service);
private:
    // Name-Service Mapping
    std::unordered_map<std::string, RpcServiceBase*> _dictionary;
    // Return value pipe
    ReturnValuePipe* _pipe;
    // on task processed
    FileDescriptorEventDelegate* _finished;
};

class RpcResultSender: public EventHandler
{
public:
    RpcResultSender(ReturnValuePipe*, FileDescriptorEventDelegate*);
    virtual ~RpcResultSender() = default;
    /**
     * If one EPOLLOUT triggered at a connfd, try prepare a response datagram and send to it
     */
    virtual void HandleWriteEvent(int fd) override;

private:
    // Return value pipe
    ReturnValuePipe* _pipe;
    // on return value sended
    FileDescriptorEventDelegate* _finished;
};
