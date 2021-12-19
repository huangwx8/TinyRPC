#include <string.h>
#include <sys/epoll.h>

#include <client/RpcClient.hh>
#include <client/CallbacksHandler.hh>

#include <common/RpcServiceProxy.hh>
#include <common/Logger.hh>

#include <transport/ClientTransport.hh>

#include <runtime/iomodel/reactor/Poller.hh>
#include <runtime/handlemodel/EventHandlerManager.hh>


static void parse_ip_port(int argc, char* argv[], const char*& ip, int& port)
{
    const char* const ip_flag = "-ip=";
    const char* const port_flag = "-port=";
    int ipflg_len = strlen(ip_flag);
    int portflg_len = strlen(port_flag);

    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], ip_flag, ipflg_len) == 0)
        {
            ip = argv[i] + ipflg_len;
        }
        else if (strncmp(argv[i], port_flag, portflg_len) == 0)
        {
            port = atoi(argv[i] + portflg_len);
        }
    }
}

static std::string parse_logfilepath(int argc, char* argv[])
{
    const char* const log_flag = "-log=";
    int log_flag_len = strlen(log_flag);

    for (int i = 1; i < argc; i++)
    {
        if (strncmp(argv[i], log_flag, log_flag_len) == 0)
        {
            return std::string(argv[i] + log_flag_len);
        }
    }

    return std::string();
}

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

RpcClient::RpcClient():
    router(nullptr),
    poller(nullptr),
    Transport(nullptr),
    Callbackshdl(nullptr),
    m_stop(false)
{

}

RpcClient::~RpcClient()
{
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
    std::unique_lock<std::mutex> initlock(m2);

    // create poller, level trigger mode
    poller = new Poller(false);
    // connection
    Transport = new ClientTransport();
    Transport->OnNoRequestToSend = [this]()
    {
        // 关闭EPOLLOUT
        poller->ModEvent(Transport->Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);
    };
    // create event handlers
    router = new EventHandlerRouter();
    Callbackshdl = new CallbacksHandler();
}

int RpcClient::Main(int argc, char* argv[])
{
    std::unique_lock<std::mutex> lock(m);
    {
        std::unique_lock<std::mutex> initlock(m2);
        const char* ip = nullptr;
        int port = -1;
        parse_ip_port(argc, argv, ip, port);

        if (ip == nullptr || port == -1)
        {
            printf("usage: %s -ip=svr_addr -port=port_number\n", basename(argv[0]));
            exit(1);
        }

        std::string log_path = parse_logfilepath(argc, argv);
        if (log_path.size() == 0)
        {
            log_path = "client.log";
        }
        log_path = "log/" + log_path;

        start_log(log_path.c_str());

        // 建立与服务器的连接
        int Connfd = Transport->Connect(ip, port);

        // 由Transport处理CLOSE事件
        router->Closehdl = Transport;
        // 由Transport处理WRITE事件
        router->Writehdl = Transport;
        // 由Callbackshdl处理READ事件
        router->Readhdl = Callbackshdl;
        // 监听所有事件
        poller->AddEvent(Connfd, EPOLLIN | EPOLLERR | EPOLLRDHUP);

        // 初始化完成，后续的所有SendRequest不会被阻塞
        initialized = true;
        c.notify_all();
    }

    // 0.1s
    int timeout = 100;

    while (!m_stop)
    {
        // wait, but not block, or this loop may dead
        auto&& tasks = poller->Dispatch(timeout, *router);
        log_dev("dispatch\n");
        // process
        for (auto&& task : tasks)
        {
            task();
        }
    }

    stop_log();

    return 0;
}

void RpcClient::SendRequest(const RpcMessage& Message, std::function<void(int)> Callback)
{
    std::unique_lock<std::mutex> initlock(m2);
    // if not initialized, wait for initialization is done
    if (!initialized)
    {
        c.wait(initlock);
    }
    // 注册回调
    Callbackshdl->CallidCallbackMapping[Message.Callid] = Callback;
    // 新请求入队
    Transport->PendingRequests.push(Message);
    // 打开EPOLLOUT
    poller->ModEvent(Transport->Connfd, EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLRDHUP);
}

void RpcClient::Bind(RpcServiceProxy* ServiceProxy)
{
    ServiceProxy->RpcPortal = this;
}
