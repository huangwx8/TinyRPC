#include <cstring>

#include <cstdio>

#include <example/EchoServer.hh>
#include <common/RpcUtils.hh>
#include <server/RpcServer.hh>


int EchoServiceImpl::Handle(const RpcMessage& Context)
{
    SERVER_EXEC_RPC_ThreeParams(Echo, string, float, int);
}

int EchoServiceImpl::Echo(const char* Message, float FloatNum, int IntegerNum)
{
    printf("Echo: Message=%s, FloatNum=%.3f, IntegerNum=%d\n", Message, FloatNum, IntegerNum);
    return 8888;
}

int main(int argc, char* argv[])
{
    EchoServiceImpl EchoServer;
    RpcServer PortalServer;

    // 初始化Rpc服务端
    PortalServer.Initialize();
    // 实现绑定到Rpc服务端
    PortalServer.RegisterService(&EchoServer);
    // 启动Rpc服务端
    PortalServer.Main(argc, argv);
}
