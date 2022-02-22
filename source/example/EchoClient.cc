#include <example/EchoClient.hh>
#include <common/RpcUtils.hh>
#include <common/Logger.hh>

std::string EchoServiceProxy::Echo(std::string data)
{
    CLIENT_CALL_RPC_OneParam(data);
    return {};
}

std::string AsyncEchoServiceProxy::Echo(std::string data)
{
    CLIENT_CALL_RPC_OneParam_Asynchronously(&AsyncEchoServiceProxy::EchoCallback, data);
    return {};
}

void AsyncEchoServiceProxy::EchoCallback(std::string return_value)
{
    log_dev("Received return value from server.\n");
    log_dev("return_value = %s\n", return_value.c_str());
}
