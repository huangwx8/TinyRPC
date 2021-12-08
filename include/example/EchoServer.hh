#pragma once

#include <example/EchoService.hh>

class EchoServiceImpl : public EchoServiceBase
{
public:
    EchoServiceImpl() = default;
    virtual ~EchoServiceImpl() = default;
    virtual void Handle(const RpcMessage& Context) override;
    virtual void Echo(const char* Message, float FloatNum, int IntegerNum) override;
};
