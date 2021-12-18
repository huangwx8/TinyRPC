#include <cstring>

#include <cstdio>

#include <example/EchoServer.hh>
#include <common/RpcUtils.hh>
#include <common/Logger.hh>
#include <server/RpcServer.hh>


int EchoServiceImpl::Handle(const RpcMessage& Context)
{
    SERVER_EXEC_RPC_ThreeParams(Echo, string, float, int);
}

int EchoServiceImpl::Echo(const char* Message, float FloatNum, int IntegerNum)
{
    log_dev("Echo: Message=%s, FloatNum=%.3f, IntegerNum=%d\n", Message, FloatNum, IntegerNum);
    return 8888;
}

int GcdServiceImpl::Handle(const RpcMessage& Context)
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

int main(int argc, char* argv[])
{
    EchoServiceImpl EchoServer;
    GcdServiceImpl GcdServer;
    RpcServer PortalServer;

    // 初始化Rpc服务端
    PortalServer.Initialize();
    // 实现绑定到Rpc服务端
    PortalServer.RegisterService(&EchoServer);
    PortalServer.RegisterService(&GcdServer);
    // 启动Rpc服务端
    PortalServer.Main(argc, argv);
}
