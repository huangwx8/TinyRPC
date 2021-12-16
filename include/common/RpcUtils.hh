#pragma once

#include <common/RpcTypes.hh>
#include <serialization/Serializer.hh>

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
void PackParam(char* Out, const char* Type, const void* In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(In, Out, Type);
    PackParam(Out + Offset, Arguments...);
}

#define INIT_RPCMESSAGE()\
RpcMessage __RpcMessage;\
bzero(&__RpcMessage, sizeof(RpcMessage));\
strcpy(__RpcMessage.RpcName, GetServiceName());

#define CLIENT_CALL_RPC()\
{\
    INIT_RPCMESSAGE()\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_OneParam(T1, P1)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), #T1, &P1);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_TwoParams(T1, P1, T2, P2)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), #T1, &P1, #T2, &P2);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_ThreeParams(T1, P1, T2, P2, T3, P3)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), #T1, &P1, #T2, &P2, #T3, &P3);\
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
    RpcImpl(Arg1, Arg2, Arg3);\
}
