# TinyRPC

- by wexuanhuang

## 整体设计

整体模块划分方式参考 trpc-cpp。

请求报文包括(callid, rpcname, rpcparams)，响应报文包括(callid, retval)。

rpcparams和retval被序列化协议序列化，序列化协议使用线性组合策略。

传输层协议使用TCP，封装Linux底层Socket系统调用。

Runtime层使用带线程池的Reactor模型，IO复用使用Epoll ET模式。

Common层实现了RPC调用接口，全局Uid管理，异步日志，定时器管理等通用功能。

仅支持异步远程调用，支持客户端绑定回调函数。

## 使用方法

为了实现远端调用，你需要一个Rpc代理类，用于客户端发起(invoke)Rpc调用；同时需要在服务器完成Rpc任务的具体实现(implement)，用于具体地执行任务并发回函数返回值。

继承`RpcServiceProxy`，填写ServiceName用于唯一标识Rpc服务名，并增加一个接口函数，声明Rpc函数的参数，返回值。

```cpp
class EchoServiceBase : public RpcServiceProxy
{
public:
    EchoServiceBase()
    {
        ServiceName = "Echo";
    }
    virtual ~EchoServiceBase() = default;
    virtual int Echo(const char* Message) = 0;
};
```

继承`ServiceBase`类，在服务端实现Rpc调用的具体实现。除了实现函数你的函数，你还需要填补Handle函数

然后，服务器的main内，注册本服务，并启动服务器；这时服务器就可响应所有到来的Rpc请求了。

```cpp
class EchoServiceImpl : public EchoServiceBase
{
public:
    virtual int Handle(const RpcMessage& Context) override
    {
        SERVER_EXEC_RPC_ThreeParams(Echo, string);
    }
    virtual int Echo(const char* Message) override
    {
        printf("Echo: %s\n", Message);
    }
};

EchoServiceImpl EchoServer;
RpcServer PortalServer;
// 初始化Rpc服务端
PortalServer.Initialize();
// 实现绑定到Rpc服务端
PortalServer.RegisterService(&EchoServer);
// 启动Rpc服务端
PortalServer.Main(argc, argv);
```

继承`ServiceBase`类，在客户端实现Rpc调用代理，只需在客户端的代理函数内添加一条宏。

异步启动Rpc客户端，绑定代理，然后便可愉快地调用Rpc了。

```cpp
class EchoServiceProxy : public EchoServiceBase
{
public:
    virtual int Echo(const char* Message) override
    {
        CLIENT_CALL_RPC_OneParam(Message);
    }
};

EchoServiceProxy EchoClient;
RpcClient PortalClient;

// 代理绑定到Rpc客户端
PortalClient.Bind(&EchoClient);
// 初始化Rpc客户端
PortalClient.Initialize();
// 在异步线程启动Rpc客户端
std::thread([&]() {
    PortalClient.Main(argc, argv);
}).detach();

// 绑定回调
EchoClient.SetCallback(
    [](int x) {
        log_dev("Echo returns [%d]\n", x);
    }
);
// 发送RPC
EchoClient.Echo("hello world!");
```
