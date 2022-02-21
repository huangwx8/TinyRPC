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
    SERVER_EXEC_RPC_ThreeParams(Echo, string, float, int);
}

std::string EchoServiceImpl::Echo(const char* Message, float FloatNum, int IntegerNum)
{
    char str[512];
    sprintf(str, "Echo: Message=%s, FloatNum=%.3f, IntegerNum=%d\n", Message, FloatNum, IntegerNum);
    log_dev(str);
    return std::string(str);
}

RpcResult GcdServiceImpl::Handle(const RpcMessage& Context)
{
    SERVER_EXEC_RPC_TwoParams(Gcd, int, int);
}

int GcdServiceImpl::Gcd(int x, int y)
{
    std::function<int (int,int)> GcdRucursive = [&GcdRucursive](int _x, int _y)
    {
        int rem = _x % _y;
        if (rem == 0)
        {
            return _y;
        }
        return GcdRucursive(_y, rem);
    };
    return x > y ? GcdRucursive(x, y) : GcdRucursive(y, x);
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
    GcdServiceImpl GcdServer;
    auto options = GetOptions(argc, argv);
    RpcServer ServerStub(options);
    
    // 实现绑定到RPC服务端
    ServerStub.RegisterService(&EchoServer);
    ServerStub.RegisterService(&GcdServer);

    // 启动RPC服务端
    ServerStub.Main(argc, argv);

    // 接受请求
    sleep(3600);
}
