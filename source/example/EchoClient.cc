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
    auto AsyncEchoPtr = ClientStub->GetProxy<AsyncEchoServiceProxy>();

    EchoPtr->Echo("hello world!");
    AsyncEchoPtr->Echo("async hello world!");

    // 等待服务器的返回值
    sleep(10);

    return 0;
}
