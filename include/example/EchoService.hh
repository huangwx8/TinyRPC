#pragma once

#include <common/RpcTypes.hh>
#include <common/RpcServiceBase.hh>

class EchoServiceBase : public RpcServiceBase
{
public:
    EchoServiceBase()
    {
        ServiceName = "Echo";
    }
    virtual ~EchoServiceBase() = default;
    virtual int Echo(const char* Message, float FloatNum, int IntegerNum) = 0;
};

class GcdServiceBase : public RpcServiceBase
{
public:
    GcdServiceBase()
    {
        ServiceName = "Gcd";
    }
    virtual ~GcdServiceBase() = default;
    virtual int Gcd(int x, int y) = 0;
};
