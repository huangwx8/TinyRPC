#include <example/EchoServer.hh>
#include <common/RpcTypes.hh>
#include <common/RpcUtils.hh>
#include <common/Logger.hh>

RpcResult EchoServiceImpl::Handle(const RpcMessage& Context)
{
    SERVER_EXEC_RPC_OneParam(Echo, string);
}

std::string EchoServiceImpl::Echo(std::string data)
{
    std::string echo_data = "Server Echo: " + data;
    log_dev("EchoServiceImpl::Echo: %s\n", echo_data.c_str());
    return echo_data;
}
