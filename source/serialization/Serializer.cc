#include <cstring>
#include <serialization/Serializer.hh>

int Serializer::Serialize(DataType InType, const void* In ,char* OutBuffer)
{
    if (InType == DataType::Int)
    {
        memcpy(OutBuffer, In, sizeof(int));
        return sizeof(int);
    }
    else if (InType == DataType::Float)
    {
        memcpy(OutBuffer, In, sizeof(float));
        return sizeof(float);
    }
    else if (InType == DataType::String)
    {
        strcpy(OutBuffer, static_cast<const char*>(In));
        return strlen(static_cast<const char*>(In)) + 1;
    }
    return -1;
}

int Serializer::Deserialize(const char* InBuffer, DataType OutType, void* Out)
{
    if (OutType == DataType::Int)
    {
        memcpy(Out, InBuffer, sizeof(int));
        return sizeof(int);
    }
    else if (OutType == DataType::Float)
    {
        memcpy(Out, InBuffer, sizeof(float));
        return sizeof(float);
    }
    else if (OutType == DataType::String)
    {
        strcpy(static_cast<char*>(Out), InBuffer);
        return strlen(static_cast<const char*>(InBuffer)) + 1;
    }
    return -1;
}
