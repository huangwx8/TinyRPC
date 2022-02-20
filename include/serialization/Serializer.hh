#pragma once

#include <common/RpcTypes.hh>

class Serializer
{
public:
    static int Serialize(const void* In ,char* Out, const char* InType);
    static int Deserialize(const char* In, void* Out, const char* OutType);

    static int Serialize(const RpcMessage* In ,char* Out);
    static int Deserialize(const char* In, RpcMessage* Out);
};
