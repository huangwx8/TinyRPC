#pragma once

#include <common/RpcTypes.hh>

class Serializer
{
public:
    static int Serialize(DataType InType, const void* In ,char* OutBuffer);
    static int Deserialize(const char* InBuffer, DataType OutType, void* Out);
};
