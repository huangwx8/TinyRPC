#include <string.h>
#include <thread>
#include <sys/epoll.h>

#include <client/RpcClient.hh>

#include <common/Logger.hh>

void EventHandlerRouter::HandleEvent(int Fd, EventType Type)
{
    switch (Type)
    {
    case CLOSE_EVENT:
        Closehdl->HandleCloseEvent(Fd);
        break;
    case READ_EVENT:
        Readhdl->HandleReadEvent(Fd);
        break;
    case WRITE_EVENT:
        Writehdl->HandleWriteEvent(Fd);
        break;
    default:
        break;
    }
}

RpcClient::RpcClient(Options _options):
    options(_options),
    _router(),
    _poller(false),
    _transport(),
    _callback_handler(),
    m_stop(false)
{
    start_log(options.log_path.c_str());
}

RpcClient::~RpcClient()
{
    stop_log();
    // Stop main()
    m_stop = true;
    // we cannot start destructor procedure before main() returns
    std::unique_lock<std::mutex> lock(m);
}

void RpcClient::Initialize()
{
    _transport.OnNoRequestToSend = [this]()
    {
        std::unique_lock<std::mutex> pollerlock(m2);
        // 关闭EPOLLOUT
        _poller.ModEvent(_transport.Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);
    };

    // 建立与服务器的连接
    int Connfd = _transport.Connect(options.svr_ip, options.svr_port);

    // 由Transport处理CLOSE事件
    _router.Closehdl = &_transport;
    // 由Transport处理WRITE事件
    _router.Writehdl = &_transport;
    // 由Callbackshdl处理READ事件
    _router.Readhdl = &_callback_handler;
    // 监听所有事件
    _poller.AddEvent(Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);

    initialized = true;
}

int RpcClient::Main(int argc, char* argv[])
{
    std::unique_lock<std::mutex> lock(m);

    // 0.1s
    int timeout = 100;

    while (!m_stop)
    {
        // wait, but not block, or this loop may dead
        auto&& tasks = _poller.Dispatch(timeout, _router);
        // process
        for (auto&& task : tasks)
        {
            task();
        }
    }

    return 0;
}

void RpcClient::SendRequest(const RpcMessage& Message, std::function<void(int)> Callback)
{
    std::unique_lock<std::mutex> sendlock(m2);
    // if not initialized, interrupt
    if (!initialized)
    {
        log_err("RpcClient::SendRequest Not initialized!\n");
        return;
    }
    // 注册回调
    _callback_handler.CallidCallbackMapping[Message.header.seqno] = Callback;
    // 新请求入队
    _transport.PendingRequests.push(Message);
    // 打开EPOLLOUT
    _poller.ModEvent(_transport.Connfd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP);
}

std::shared_ptr<RpcClient> RpcClient::GetRpcClient(Options options)
{
    std::shared_ptr<RpcClient> sp = std::make_shared<RpcClient>(options);
    // 初始化Rpc客户端
    sp->Initialize();
    // 启动Rpc客户端
    std::thread([&]() {
        sp->Main(1, nullptr);
    }).detach();
    return sp; 
}
