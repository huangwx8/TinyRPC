#include <string.h>
#include <thread>
#include <sys/epoll.h>

#include <client/RpcClient.hh>
#include <client/CallbacksHandler.hh>

#include <common/RpcServiceBase.hh>
#include <common/Logger.hh>

#include <transport/ClientTransport.hh>

#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>




/**
 * 中转站
 */
class EventHandlerRouter: public EventHandler 
{
public:
    EventHandlerRouter():
        Readhdl(nullptr),
        Writehdl(nullptr),
        Closehdl(nullptr)
    {}

    virtual ~EventHandlerRouter() {}

    virtual void HandleEvent(int Fd, EventType Type) override;

    EventHandler* Readhdl;
    EventHandler* Writehdl;
    EventHandler* Closehdl;
};

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
    router(nullptr),
    poller(nullptr),
    Transport(nullptr),
    Callbackshdl(nullptr),
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
    if (router)
    {
        delete router;
        router = nullptr;
    }
    if (poller)
    {
        delete poller;
        poller = nullptr;
    }
    if (Transport)
    {
        delete Transport;
        Transport = nullptr;
    }
    if (Callbackshdl)
    {
        delete Callbackshdl;
        Callbackshdl = nullptr;
    }
}

void RpcClient::Initialize()
{
    // create poller, level trigger mode
    poller = new Poller(false);
    // connection
    Transport = new ClientTransport();
    Transport->OnNoRequestToSend = [this]()
    {
        std::unique_lock<std::mutex> pollerlock(m2);
        // 关闭EPOLLOUT
        poller->ModEvent(Transport->Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);
    };
    // create event handlers
    router = new EventHandlerRouter();
    Callbackshdl = new CallbacksHandler();

    // 建立与服务器的连接
    int Connfd = Transport->Connect(options.svr_ip, options.svr_port);

    // 由Transport处理CLOSE事件
    router->Closehdl = Transport;
    // 由Transport处理WRITE事件
    router->Writehdl = Transport;
    // 由Callbackshdl处理READ事件
    router->Readhdl = Callbackshdl;
    // 监听所有事件
    poller->AddEvent(Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);

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
        auto&& tasks = poller->Dispatch(timeout, *router);
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
    Callbackshdl->CallidCallbackMapping[Message.Callid] = Callback;
    // 新请求入队
    Transport->PendingRequests.push(Message);
    // 打开EPOLLOUT
    poller->ModEvent(Transport->Connfd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP);
}

RpcClient& RpcClient::GetRpcClient(Options options)
{
    static RpcClient ClientStub(options);
    // 初始化Rpc客户端
    ClientStub.Initialize();
    // 启动Rpc客户端
    std::thread([&]() {
        ClientStub.Main(1, nullptr);
    }).detach();
    return ClientStub; 
}
