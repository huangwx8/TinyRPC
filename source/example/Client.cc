#include <cstring>
#include <unistd.h>

#include <common/Logger.hh>
#include <common/ArgsParser.hh>

#include <client/RpcClient.hh>

#include <example/NameClient.hh>

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

    auto Getter = ClientStub->GetProxy<NameGetterProxy>();
    auto Setter = ClientStub->GetProxy<NameSetterProxy>();

    Setter->SetName(8888, "history of mistery");
    Getter->GetName(8888);

    sleep(10);

    return 0;
}
