#include <string>
#include <cstring>
#include <cstdio>
#include <unistd.h>

#include <example/EchoServer.hh>
#include <common/RpcUtils.hh>
#include <common/Logger.hh>
#include <common/ArgsParser.hh>
#include <server/RpcServer.hh>


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

static RpcServer::Options GetOptions(int argc, char* argv[])
{
    const char* ip = parse_str(argc, argv, "-ip=");
    int port = parse_int(argc, argv, "-port=");
    const char* log_path = parse_str(argc, argv, "-log=");
    if (ip == nullptr)
    {
        ip = "localhost";
    }
    if (port == -1)
    {
        port = 8888;
    }
    if (log_path == nullptr)
    {
        log_path = "server.log";
    }
    return RpcServer::Options{.ip_addr = std::string(ip), .port = port, .log_path = "log/" + std::string(log_path)};
}

int main(int argc, char* argv[])
{
    EchoServiceImpl EchoServer;
    auto options = GetOptions(argc, argv);
    RpcServer ServerStub(options);
    
    // 实现绑定到RPC服务端
    ServerStub.RegisterService(&EchoServer);

    // 启动RPC服务端
    ServerStub.Main(argc, argv);

    // 接受请求
    sleep(3600);
}
