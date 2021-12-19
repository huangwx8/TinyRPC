#include <cstring>
#include <future>

#include <unistd.h>

#include <example/EchoClient.hh>
#include <common/RpcTypes.hh>
#include <common/RpcUtils.hh>
#include <common/Logger.hh>
#include <common/Timer.hh>
#include <client/RpcClient.hh>

int EchoServiceProxy::Echo(const char* Message, float FloatNum, int IntegerNum)
{
    CLIENT_CALL_RPC_ThreeParams(Message, FloatNum, IntegerNum);
    return 0;
}

int GcdServiceProxy::Gcd(int x, int y)
{
    CLIENT_CALL_RPC_TwoParams(x ,y);
    return 0;
}

int main(int argc, char* argv[])
{
    EchoServiceProxy EchoClient;
    GcdServiceProxy GcdClient;
    RpcClient PortalClient;
    GlobalTimer timer;

    // 代理绑定到Rpc客户端
    PortalClient.Bind(&EchoClient);
    PortalClient.Bind(&GcdClient);
    // 初始化Rpc客户端
    PortalClient.Initialize();
    // 启动Rpc客户端
    std::thread([&]() {
        PortalClient.Main(argc, argv);
    }).detach();

    // 绑定回调
    EchoClient.SetCallback(
        [](int x) {
            log_dev("Echo returns [%d]\n", x);
        }
    );
    GcdClient.SetCallback(
        [](int x) {
            log_dev("Gcd is [%d]\n", x);
        }
    );
    // 发送RPC
    EchoClient.Echo("fuck c++", 114.514, 1919810);
    EchoClient.Echo("thanku c++", 3.14159, 198434);
    GcdClient.Gcd(1344, 42);

    // 等待服务器的返回值
    sleep(10);

    return 0;
}
