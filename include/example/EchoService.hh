#pragma once

#include <string>

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
    virtual std::string Echo(std::string) = 0;
};
