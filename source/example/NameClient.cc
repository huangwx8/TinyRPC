#include <functional>

#include <example/NameClient.hh>

#include <common/RpcUtils.hh>
#include <common/Logger.hh>

std::string NameGetterProxy::GetName(int k)
{
    std::function<void(std::string)> cb = [=](std::string s) {
        log_dev("GetName(%d) from server: %s\n", k, s.c_str());
    };
    CLIENT_CALL_RPC_OneParam_Asynchronously(cb, k);
    return {};
}

int NameSetterProxy::SetName(int k, std::string v)
{
    CLIENT_CALL_RPC_TwoParams(k ,v);
    return 0;
}
