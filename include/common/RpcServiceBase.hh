#pragma once

#include <functional>
#include <common/RpcTypes.hh>

#ifdef BUILD_CLIENT
#include <client/RpcClient.hh>
#include <client/CallbacksHandler.hh>
#endif

class RpcServiceBase
{
    /** Common section */
public:
    RpcServiceBase() {};
    virtual ~RpcServiceBase() = default;
    const char* GetServiceName() { return ServiceName; };
protected:
    const char* ServiceName = "None";

#ifdef BUILD_SERVER
    /** Server section */
public:
    virtual RpcResult Handle(const RpcMessage& Context) = 0;
#endif

#ifdef BUILD_CLIENT
friend RpcClient;
    /** Client section */
public:
    void Invoke(const RpcMessage& Context)
    {
        if (client) 
        {
            client->SendRequest(Context);
        }
    }

    template<typename T>
    void AsyncInvoke(RpcMessage Context, T func)
    {
        if (client) 
        {
            // 注册回调
            client->GetCallbacksHandler().Register(Context.header.seqno, func);
            // 发送报文
            Context.header.need_return = true;
            client->SendRequest(Context);
        }
    }
private:
    RpcClient* client = nullptr;
#endif
};
