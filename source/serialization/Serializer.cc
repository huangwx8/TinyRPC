#include <cstring>
#include <serialization/Serializer.hh>

int Serializer::Serialize(const void* In ,char* Out, const char* InType)
{
    if (strcmp(InType, INT_TYPE) == 0)
    {
        memcpy(Out, In, sizeof(int));
        return sizeof(int);
    }
    else if (strcmp(InType, FLOAT_TYPE) == 0)
    {
        memcpy(Out, In, sizeof(float));
        return sizeof(float);
    }
    else if (strcmp(InType, STRING_TYPE) == 0)
    {
        strcpy(Out, static_cast<const char*>(In));
        return strlen(static_cast<const char*>(In)) + 1;
    }
    return -1;
}

int Serializer::Deserialize(const char* In, void* Out, const char* OutType)
{
    if (strcmp(OutType, INT_TYPE) == 0)
    {
        memcpy(Out, In, sizeof(int));
        return sizeof(int);
    }
    else if (strcmp(OutType, FLOAT_TYPE) == 0)
    {
        memcpy(Out, In, sizeof(float));
        return sizeof(float);
    }
    else if (strcmp(OutType, STRING_TYPE) == 0)
    {
        strcpy(static_cast<char*>(Out), In);
        return strlen(static_cast<const char*>(In)) + 1;
    }
    return -1;
}

int Serializer::Serialize(const RpcMessage* In ,char* Out)
{
    memcpy(Out, &In->header, sizeof(RpcHeader));
    memcpy(Out, &In->body, In->header.body_length);
    return sizeof(RpcHeader) + In->header.body_length;
}

int Serializer::Deserialize(const char* In, RpcMessage* Out)
{
    memcpy(&Out->header, In, sizeof(RpcHeader));
    memcpy(&Out->body, (In + sizeof(RpcHeader)), Out->header.body_length);
    return sizeof(RpcHeader) + Out->header.body_length;
}
