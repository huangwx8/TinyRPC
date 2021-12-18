#pragma once

#include <common/RpcTypes.hh>
#include <common/RpcServiceProxy.hh>

class EchoServiceBase : public RpcServiceProxy
{
public:
    EchoServiceBase()
    {
        ServiceName = "Echo";
    }
    virtual ~EchoServiceBase() = default;
    virtual int Echo(const char* Message, float FloatNum, int IntegerNum) = 0;
};
