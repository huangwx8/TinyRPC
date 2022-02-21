#pragma once

#include <string>
#include <functional>

#include <example/EchoService.hh>

class EchoServiceProxy : public EchoServiceBase
{
public:
    EchoServiceProxy() = default;
    virtual ~EchoServiceProxy() = default;
    virtual std::string Echo(std::string data) override;
};

class AsyncEchoServiceProxy : public EchoServiceBase
{
public:
    AsyncEchoServiceProxy() = default;
    virtual ~AsyncEchoServiceProxy() = default;
    virtual std::string Echo(std::string data) override;
private:
    static void EchoCallback(std::string return_value);
};
