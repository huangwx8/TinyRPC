#pragma once

#include <common/RpcTypes.hh>
#include <serialization/Serializer.hh>

void ParseParam(const char* In);

template<typename... ArgumentTypes>
void ParseParam(const char* In, DataType Type, void* Out, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Deserialize(In, Type, Out);
    ParseParam(In + Offset, Arguments...);
}

void PackParam(char* Out);

template<typename... ArgumentTypes>
void PackParam(char* Out, DataType Type, const void* In, ArgumentTypes... Arguments)
{
    int Offset = Serializer::Serialize(Type, In, Out);
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

#define CLIENT_CALL_RPC_OneParam(RPCT1, P1)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), RPCT1, P1);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_TwoParams(RPCT1, P1, RPCT2, P2)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), RPCT1, P1, RPCT2, P2);\
    Invoke(__RpcMessage);\
}

#define CLIENT_CALL_RPC_ThreeParams(RPCT1, P1, RPCT2, P2, RPCT3, P3)\
{\
    INIT_RPCMESSAGE()\
    PackParam(&(__RpcMessage.RpcParameters[0]), RPCT1, P1, RPCT2, P2, RPCT3, P3);\
    Invoke(__RpcMessage);\
}

#define SERVER_EXEC_RPC(RpcImpl)\
{\
    RpcImpl();\
}

#define SERVER_EXEC_RPC_OneParam(RpcImpl, T1, RPCT1)\
{\
    T1 Arg1;\
    ParseParam(&(Context.RpcParameters[0]), RPCT1, &Arg1);\
    RpcImpl(Arg1);\
}

#define SERVER_EXEC_RPC_TwoParams(RpcImpl, T1, RPCT1, T2, RPCT2)\
{\
    T1 Arg1;\
    T2 Arg2;\
    ParseParam(&(Context.RpcParameters[0]), RPCT1, &Arg1, RPCT2, &Arg2);\
    RpcImpl(Arg1, Arg2);\
}

#define SERVER_EXEC_RPC_ThreeParams(RpcImpl, T1, RPCT1, T2, RPCT2, T3, RPCT3)\
{\
    T1 Arg1;\
    T2 Arg2;\
    T3 Arg3;\
    ParseParam(&(Context.RpcParameters[0]), RPCT1, &Arg1, RPCT2, &Arg2, RPCT3, &Arg3);\
    RpcImpl(Arg1, Arg2, Arg3);\
}
