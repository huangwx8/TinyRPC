#pragma once

#include <string>

#include <example/EchoService.hh>

class EchoServiceImpl : public EchoServiceBase
{
public:
    EchoServiceImpl() = default;
    virtual ~EchoServiceImpl() = default;
    virtual RpcResult Handle(const RpcMessage& Context) override;
    virtual std::string Echo(const char* Message, float FloatNum, int IntegerNum) override;
};

class GcdServiceImpl : public GcdServiceBase
{
public:
    GcdServiceImpl() = default;
    virtual ~GcdServiceImpl() = default;
    virtual RpcResult Handle(const RpcMessage& Context) override;
    virtual int Gcd(int x, int y) override;
};
