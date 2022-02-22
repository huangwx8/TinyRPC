#pragma once

#include <example/NameService.hh>

class NameGetterImpl : public NameGetterBase
{
public:
    NameGetterImpl(NameServiceBase* base): _base(base) {}
    virtual ~NameGetterImpl() = default;
    virtual RpcResult Handle(const RpcMessage& Context) override;
    virtual std::string GetName(int) override;
private:
    NameServiceBase* _base;
};

class NameSetterImpl : public NameSetterBase
{
public:
    NameSetterImpl(NameServiceBase* base): _base(base) {}
    virtual ~NameSetterImpl() = default;
    virtual RpcResult Handle(const RpcMessage& Context) override;
    virtual int SetName(int, std::string) override;
private:
    NameServiceBase* _base;
};
