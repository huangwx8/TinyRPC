#include <cstring>
#include <future>

#include <unistd.h>

#include <example/EchoClient.hh>
#include <common/RpcTypes.hh>
#include <common/RpcUtils.hh>
#include <common/Logger.hh>
#include <common/Timer.hh>
#include <common/ArgsParser.hh>
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

static RpcClient::Options GetOptions(int argc, char* argv[])
{
    const char* ip = parse_str(argc, argv, "-ip=");
    int port = parse_int(argc, argv, "-port=");
    if (ip == nullptr || port == -1)
    {
        printf("usage: %s -ip=svr_addr -port=port_number\n", basename(argv[0]));
        exit(1);
    }
    const char* log_path = parse_str(argc, argv, "-log=");

    return RpcClient::Options{.svr_ip = std::string(ip), .svr_port = port, .log_path = log_path ? "log/" + std::string(log_path) : std::string()};
}

int main(int argc, char* argv[])
{
    auto options = GetOptions(argc, argv);
    auto&& ClientStub = RpcClient::GetRpcClient(options);

    auto EchoPtr = ClientStub->GetProxy<EchoServiceProxy>();
    EchoPtr->SetCallback(
        [](int x) {
            log_dev("Echo returns [%d]\n", x);
        }
    );
    EchoPtr->Echo("fuck c++", 114.514, 1919810);
    EchoPtr->Echo("thanku c++", 3.14159, 198434);

    auto GcdPtr = ClientStub->GetProxy<GcdServiceProxy>();
    GcdPtr->SetCallback(
        [](int x) {
            log_dev("Gcd is [%d]\n", x);
        }
    );
    GcdPtr->Gcd(1344, 42);

    // 等待服务器的返回值
    sleep(10);

    return 0;
}
