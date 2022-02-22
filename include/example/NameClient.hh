#pragma once

#include <example/NameService.hh>

class NameGetterProxy : public NameGetterBase
{
public:
    NameGetterProxy() = default;
    virtual ~NameGetterProxy() = default;
    virtual std::string GetName(int) override;
};

class NameSetterProxy : public NameSetterBase
{
public:
    NameSetterProxy() = default;
    virtual ~NameSetterProxy() = default;
    virtual int SetName(int, std::string) override;
};
