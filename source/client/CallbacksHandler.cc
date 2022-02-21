// std c
#include <cstdio>
#include <cstring>

// linux
#include <sys/socket.h>

#include <client/CallbacksHandler.hh>

#include <common/RpcUtils.hh>
#include <common/RpcTypes.hh>
#include <common/Guid.hh>
#include <common/Defines.hh>
#include <common/Logger.hh>

static void ErrorCallback(int retval)
{
    log_err("Uninitialized callback function, client may receive a bad rpc response packet.");
    exit(1);
}

CallbackFunction::CallbackFunction():
    IntCallback(nullptr),
    FloatCallback(nullptr),
    StringCallback(nullptr)
{

}

CallbackFunction::~CallbackFunction()
{
    Free();
}

void CallbackFunction::Exec(int i) 
{
    if (IntCallback)
    {
        (*IntCallback)(i);
    }
}

void CallbackFunction::Exec(float f) 
{
    if (FloatCallback)
    {
        (*FloatCallback)(f);
    }
}

void CallbackFunction::Exec(std::string s) 
{
    if (StringCallback)
    {
        (*StringCallback)(s);
    }
}

void CallbackFunction::Register(std::function<void(int)> func)
{
    IntCallback = new std::function<void(int)> (func);
}

void CallbackFunction::Register(std::function<void(float)> func)
{
    FloatCallback = new std::function<void(float)> (func);
}

void CallbackFunction::Register(std::function<void(std::string)> func)
{
    StringCallback = new std::function<void(std::string)> (func);
}

void CallbackFunction::Free()
{
    if (IntCallback) 
    {
        delete IntCallback;
        IntCallback = nullptr;
    }
    if (FloatCallback) 
    {
        delete FloatCallback;
        FloatCallback = nullptr;
    }
    if (StringCallback) 
    {
        delete StringCallback;
        StringCallback = nullptr;
    }
}

CallbacksHandler::CallbacksHandler():
    CallidCallbackMapping(MAX_REQUESTS, CallbackFunction())
{
    
}

CallbacksHandler::~CallbacksHandler()
{

}

void CallbacksHandler::HandleReadEvent(int Fd)
{
    RpcResult res;
    // bzero receive buffer and receive
    bzero(&res, sizeof(RpcResult));
    int ret = recv(Fd, &res, sizeof(RpcResult), 0);
    if (ret == -1)
    {
        log_dev("CallbacksHandler::HandleReadEvent: receive failed.\n");
        return;
    }

    if (res.type == 1) 
    {
        int i;
        ParseParam(res.return_buffer, "int", &i);
        CallidCallbackMapping[res.seqno].Exec(i);
    }
    else if (res.type == 2)
    {
        float f;
        ParseParam(res.return_buffer, "float", &f);
        CallidCallbackMapping[res.seqno].Exec(f);
    }
    else if (res.type == 3) 
    {
        std::string s(res.return_buffer);
        CallidCallbackMapping[res.seqno].Exec(s);
    }

    Guid::RecycleGuid(res.seqno);
}

void CallbacksHandler::Register(int seqno, std::function<void(int)> callback) 
{
    CallidCallbackMapping[seqno].Register(callback);
}

void CallbacksHandler::Register(int seqno, std::function<void(float)> callback) 
{
    CallidCallbackMapping[seqno].Register(callback);
}

void CallbacksHandler::Register(int seqno, std::function<void(std::string)> callback) 
{
    CallidCallbackMapping[seqno].Register(callback);
}
