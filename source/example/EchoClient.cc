#include <cstring>

#include <unistd.h>

#include <example/EchoClient.hh>
#include <common/RpcTypes.hh>
#include <common/RpcUtils.hh>
#include <client/RpcClient.hh>

void EchoServiceProxy::Echo(const char* Message, float FloatNum, int IntegerNum)
{
    CLIENT_CALL_RPC_ThreeParams(Message, FloatNum, IntegerNum);
}

int main(int argc, char* argv[])
{
    EchoServiceProxy EchoClient;
    RpcClient PortalClient;

    // 代理绑定到Rpc客户端
    PortalClient.Bind(&EchoClient);
    // 初始化Rpc客户端
    PortalClient.Initialize();
    // 启动Rpc客户端
    PortalClient.Main(argc, argv);
    // 发送RPC
    EchoClient.Echo("fuck c++", 114.514, 1919810);

    sleep(1);
}
