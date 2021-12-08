#pragma once

#include <example/EchoService.hh>

class EchoServiceProxy : public EchoServiceBase
{
public:
    EchoServiceProxy() = default;
    virtual ~EchoServiceProxy() = default;
    virtual void Echo(const char* Message, float FloatNum, int IntegerNum) override;
};
