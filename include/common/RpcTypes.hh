#pragma once

#define MAX_RPC_NAME_SIZE 32
#define MAX_RPC_PARAMS_SIZE 480

#define INT_TYPE "int"
#define FLOAT_TYPE "float"
#define STRING_TYPE "string"

struct RpcMessage
{
    int Callid;
    char RpcName[MAX_RPC_NAME_SIZE];
    char RpcParameters[MAX_RPC_PARAMS_SIZE]; 
};

struct RpcResult
{
    int Callid;
    int RetVal;
};
