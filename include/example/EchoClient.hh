#pragma once

#include <string>
#include <functional>

#include <example/EchoService.hh>

class EchoServiceProxy : public EchoServiceBase
{
public:
    EchoServiceProxy() = default;
    virtual ~EchoServiceProxy() = default;
    virtual std::string Echo(const char* Message, float FloatNum, int IntegerNum) override;
};

class AsyncEchoServiceProxy : public EchoServiceBase
{
public:
    AsyncEchoServiceProxy(std::function<void(std::string)> f) { _callback = f; };
    virtual ~AsyncEchoServiceProxy() = default;
    virtual std::string Echo(const char* Message, float FloatNum, int IntegerNum) override;
private:
    std::function<void(std::string)> _callback;
};

class GcdServiceProxy : public GcdServiceBase
{
public:
    GcdServiceProxy() = default;
    virtual ~GcdServiceProxy() = default;
    virtual int Gcd(int x, int y) override;
};

class AsyncGcdServiceProxy : public GcdServiceBase
{
public:
    AsyncGcdServiceProxy(std::function<void(int)> f) { _callback = f; };
    virtual ~AsyncGcdServiceProxy() = default;
    virtual int Gcd(int x, int y) override;
private:
    std::function<void(int)> _callback;
};
