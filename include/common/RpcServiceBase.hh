#pragma once

#include <functional>
#include <common/RpcTypes.hh>

class RpcServiceBase
{
    /** Common section */
public:
    RpcServiceBase();
    virtual ~RpcServiceBase() = default;
    const char* GetServiceName();
protected:
    const char* ServiceName = "None";

#ifdef BUILD_SERVER
    /** Server section */
public:
    virtual int Handle(const RpcMessage& Context);
#endif

#ifdef BUILD_CLIENT
    /** Client section */
    using FCallback = std::function<void(int)>;
    using FSendChannel = std::function<void(const RpcMessage&, FCallback)>;
public:
    virtual void Invoke(const RpcMessage& Context);
    void SetCallback(const FCallback&);
    void SetSendChannel(const FSendChannel&);
private:
    FSendChannel SendChannel;
    FCallback Callback;
#endif
};
