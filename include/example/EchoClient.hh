#pragma once

#include <example/EchoService.hh>

class EchoServiceProxy : public EchoServiceBase
{
public:
    EchoServiceProxy() = default;
    virtual ~EchoServiceProxy() = default;
    virtual int Echo(const char* Message, float FloatNum, int IntegerNum) override;
};

class GcdServiceProxy : public GcdServiceBase
{
public:
    GcdServiceProxy() = default;
    virtual ~GcdServiceProxy() = default;
    virtual int Gcd(int x, int y) override;
};
