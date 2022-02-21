#pragma once

#include <string>

#include <example/EchoService.hh>

class EchoServiceImpl : public EchoServiceBase
{
public:
    EchoServiceImpl() = default;
    virtual ~EchoServiceImpl() = default;
    virtual RpcResult Handle(const RpcMessage& Context) override;
    virtual std::string Echo(std::string data) override;
};
