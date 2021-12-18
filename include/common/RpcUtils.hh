#pragma once

#include <common/RpcTypes.hh>
#include <common/Guid.hh>
#include <serialization/Serializer.hh>

#include <stdio.h>

using string = char[512];

void ParseParam(const char* In);

template<typename... ArgumentTypes>
void ParseParam(const char* In, const char* Type, void* Out, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Deserialize(In, Out, Type);
    ParseParam(In + Offset, Arguments...);
}

void PackParam(char* Out);

template<typename... ArgumentTypes>
void PackParam(char* Out, const int& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "int");
    PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
void PackParam(char* Out, int& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "int");
    PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
void PackParam(char* Out, const float& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "float");
    PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
void PackParam(char* Out, float& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(&In, Out, "float");
    PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
void PackParam(char* Out, char*& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(In, Out, "string");
    PackParam(Out + Offset, Arguments...);
}

template<typename... ArgumentTypes>
void PackParam(char* Out, const char*& In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(In, Out, "string");
    PackParam(Out + Offset, Arguments...);
}


#define INIT_RPCMESSAGE()\
RpcMessage __RpcMessage;\
bzero(&__RpcMessage, sizeof(RpcMessage));\
__RpcMessage.Callid = Guid::GetGuid();\
strcpy(__RpcMessage.RpcName, GetServiceName());

#define CLIENT_CALL_RPC()\
{\
    INIT_RPCMESSAGE()\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_OneParam(P1)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), P1);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_TwoParams(P1, P2)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), P1, P2);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_ThreeParams(P1, P2, P3)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), P1, P2, P3);\
    Invoke(__RpcMessage);\
}

#define SERVER_EXEC_RPC(RpcImpl)\
{\
    RpcImpl();\
}

#define SERVER_EXEC_RPC_OneParam(RpcImpl, T1)\
{\
    T1 Arg1;\
    ParseParam(&(Context.RpcParameters[0]), #T1, &Arg1);\
    RpcImpl(Arg1);\
}

#define SERVER_EXEC_RPC_TwoParams(RpcImpl, T1, T2)\
{\
    T1 Arg1;\
    T2 Arg2;\
    ParseParam(&(Context.RpcParameters[0]), #T1, &Arg1, #T2, &Arg2);\
    RpcImpl(Arg1, Arg2);\
}

#define SERVER_EXEC_RPC_ThreeParams(RpcImpl, T1, T2, T3)\
{\
    T1 Arg1;\
    T2 Arg2;\
    T3 Arg3;\
    ParseParam(&(Context.RpcParameters[0]), #T1, &Arg1, #T2, &Arg2, #T3, &Arg3);\
    return RpcImpl(Arg1, Arg2, Arg3);\
}
