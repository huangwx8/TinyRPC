#pragma once

#define MAX_RPC_NAME_SIZE 32
#define MAX_RPC_PARAMS_SIZE 480

enum class DataType
{
    Int, 
    Float, 
    String
};

struct RpcMessage
{
    char RpcName[MAX_RPC_NAME_SIZE];
    char RpcParameters[MAX_RPC_PARAMS_SIZE]; 
};
