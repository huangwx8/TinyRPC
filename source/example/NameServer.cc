#include <example/NameServer.hh>

#include <common/RpcUtils.hh>
#include <common/Logger.hh>

RpcResult NameGetterImpl::Handle(const RpcMessage& Context)
{
    SERVER_EXEC_RPC_OneParam(GetName, int);
}

std::string NameGetterImpl::GetName(int k)
{
    auto ret = _base->GetNameInternal(k);
    log_dev("NameGetterImpl::GetName return %s\n", ret.c_str());
    return ret;
}

RpcResult NameSetterImpl::Handle(const RpcMessage& Context)
{
    SERVER_EXEC_RPC_TwoParams(SetName, int, string);
}

int NameSetterImpl::SetName(int k, std::string v)
{
    auto ret = _base->SetNameInternal(k, v);
    log_dev("NameSetterImpl::SetName return %d\n", ret);
    return ret;
}
