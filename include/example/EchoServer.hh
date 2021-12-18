#pragma once

#include <example/EchoService.hh>

class EchoServiceImpl : public EchoServiceBase
{
public:
    EchoServiceImpl() = default;
    virtual ~EchoServiceImpl() = default;
    virtual int Handle(const RpcMessage& Context) override;
    virtual int Echo(const char* Message, float FloatNum, int IntegerNum) override;
};
