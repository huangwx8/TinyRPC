#pragma once

#include <common/RpcTypes.hh>
#include <serialization/Serializer.hh>
#include <string.h>

using string = char[512];

void ParseParam(const char* In);

template<typename... ArgumentTypes>
void ParseParam(const char* In, const char* Type, void* Out, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Deserialize(In, Out, Type);
    ParseParam(In + Offset, Arguments...);
}

int PackParam(char* Out);

template<typename... ArgumentTypes>
int PackParam(char* Out, const int& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "int");
    return Offset + PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
int PackParam(char* Out, int& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "int");
    return Offset + PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
int PackParam(char* Out, const float& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "float");
    return Offset + PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
int PackParam(char* Out, float& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "float");
    return Offset + PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
int PackParam(char* Out, char*& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(In, Out, "string");
    return Offset + PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
int PackParam(char* Out, const char*& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(In, Out, "string");
    return Offset + PackParam(Out + Offset, Arguments...);
}


#define INIT_RPCMESSAGE()\
RpcMessage __RpcMessage;\
strcpy(__RpcMessage.header.servicename, GetServiceName());\

#define CLIENT_CALL_RPC()\
{\
    INIT_RPCMESSAGE()\
    __RpcMessage.header.body_length = 0;\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_OneParam(P1)\
{\
    INIT_RPCMESSAGE()\
    __RpcMessage.header.body_length = PackParam(&(__RpcMessage.body.parameters[0]), P1);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_TwoParams(P1, P2)\
{\
    INIT_RPCMESSAGE()\
    __RpcMessage.header.body_length = PackParam(&(__RpcMessage.body.parameters[0]), P1, P2);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_ThreeParams(P1, P2, P3)\
{\
    INIT_RPCMESSAGE()\
    __RpcMessage.header.body_length = PackParam(&(__RpcMessage.body.parameters[0]), P1, P2, P3);\
    Invoke(__RpcMessage);\
}

#define SERVER_EXEC_RPC(RpcImpl)\
{\
    RpcImpl();\
}

#define SERVER_EXEC_RPC_OneParam(RpcImpl, T1)\
{\
    T1 Arg1;\
    ParseParam(&(Context.body.parameters[0]), #T1, &Arg1);\
    return RpcImpl(Arg1);\
}

#define SERVER_EXEC_RPC_TwoParams(RpcImpl, T1, T2)\
{\
    T1 Arg1;\
    T2 Arg2;\
    ParseParam(&(Context.body.parameters[0]), #T1, &Arg1, #T2, &Arg2);\
    return RpcImpl(Arg1, Arg2);\
}

#define SERVER_EXEC_RPC_ThreeParams(RpcImpl, T1, T2, T3)\
{\
    T1 Arg1;\
    T2 Arg2;\
    T3 Arg3;\
    ParseParam(&(Context.body.parameters[0]), #T1, &Arg1, #T2, &Arg2, #T3, &Arg3);\
    return RpcImpl(Arg1, Arg2, Arg3);\
}
