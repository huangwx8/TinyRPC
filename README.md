# TinyRPC

## 整体设计

整体模块划分方式参考 trpc-cpp。

请求报文和响应报文的结构定义如下

```cpp
struct RpcHeader
{
    unsigned short magic;
    unsigned short version;
    bool need_return;
    int seqno;
    int body_length;
    char servicename[MAX_RPC_NAME_SIZE];
};

struct RpcBody
{
    char parameters[MAX_RPC_PARAMS_SIZE];
};

struct RpcResult
{
    int seqno;
    int type; // 1 - int, 2 - float, 3 - string
    char return_buffer[MAX_RPC_RETURN_VALUE];
};
```

其中magic用于服务器，以快速过滤不属于本RPC协议的数据包；servicename是服务名，用于服务器识别请求类型，并派发给对应的例程；单次调用的请求和响应共享一个seqno，用于客户端分发返回值到不同的RPC协议；need_return用于标识服务器是否需要发回函数的返回值；parameters是请求体，包含函数参数；type用于客户端区分返回值类型，return_buffer包含函数返回值。

Serialization层实现了序列化方法，用于RPC读写parameters和return_buffer两个字段。

Runtime层参考 libevent，实现了带线程池的 Reactor+Epoll ET 实现io复用，对触发的事件，定义了事件处理基类EventHandler，方便上层继承重写处理逻辑。

Transport层封装Linux底层Socket系统调用，在服务器和客户端间建立TCP通道，并负责把接受到的数据转给Runtime层。

Common层实现了RPC调用接口，全局Uid管理，异步日志，定时器管理等通用功能。

## 使用方法

这里通过演示一个简单的客户端调用服务器的helloworld的用法，简单介绍本框架的使用方法。

为了实现远端调用，你需要一个Rpc代理类，用于客户端发起(invoke)Rpc调用；同时需要在服务器完成Rpc任务的具体实现(implement)，用于具体地执行任务并发回函数返回值。

### 编写服务基类

继承`RpcServiceBase`，填写ServiceName用于唯一标识Rpc服务名，并增加一个接口函数，声明Rpc函数的参数，返回值。

```cpp
class EchoServiceBase : public RpcServiceBase
{
public:
    EchoServiceBase()
    {
        ServiceName = "Echo";
    }
    virtual ~EchoServiceBase() = default;
    virtual std::string Echo(std::string) = 0;
};
```

### 编写服务具体实现

继承`EchoServiceBase`类，在服务端实现Rpc调用的具体实现。这里的一个不足点是，你需要填写一下Handle函数的实现，这是RPC的统一处理入口。

然后，服务器的main内，启动服务器并注册服务；这时服务器就可响应所有到来的Rpc请求了。

```cpp
class EchoServiceImpl : public EchoServiceBase
{
public:
    EchoServiceImpl() = default;
    virtual ~EchoServiceImpl() = default;
    virtual RpcResult Handle(const RpcMessage& Context) override;
    virtual std::string Echo(std::string data) override;
};

RpcResult EchoServiceImpl::Handle(const RpcMessage& Context)
{
    SERVER_EXEC_RPC_OneParam(Echo, string);
}

std::string EchoServiceImpl::Echo(std::string data)
{
    std::string echo_data = "Server says: " + data;
    std::cout << echo_data << std::endl;
    return echo_data;
}


int main(int argc, char* argv[])
{
    RpcServer ServerStub({"127.0.0.1", 8888, "server.log"}); // 获取一个RPC Server对象
    EchoServiceImpl EchoImplementation; // 构造一个Echo服务的实现
    ServerStub.RegisterService(&EchoImplementation); // 注册Echo服务
    ServerStub.Main(argc, argv); // 运行RPC Server，等待并处理请求
}
```

### 实现单向调用

继承`EchoServiceBase`类，在客户端实现Rpc调用代理，你只需在客户端的代理函数内添加一条宏。

启动Rpc客户端，调用客户端方法获取服务代理对象，然后你将可以看到服务器打印了 "Server says: hello world!"。

```cpp
class EchoServiceProxy : public EchoServiceBase
{
public:
    EchoServiceProxy() = default;
    virtual ~EchoServiceProxy() = default;
    virtual std::string Echo(std::string data) override;
};

std::string EchoServiceProxy::Echo(std::string data)
{
    CLIENT_CALL_RPC_OneParam(data);
    return {};
}

int main(int argc, char* argv[])
{
    auto&& ClientStub = RpcClient::GetRpcClient({ "127.0.0.1", 8888, "client.log" }); // 获取一个RPC Client对象
    auto EchoPtr = ClientStub->GetProxy<EchoServiceProxy>();// 获取一个RPC Proxy对象
    EchoPtr->Echo("hello world!"); // 发起调用
    return 0;
}
```

### 使用回调

上面的情况下，我们在客户端调用了远程的函数，但是客户端没有得到返回值。如果想编写能获取并处理返回值的客户端程序，只需对上面的客户端代码稍作修改。

```cpp
class AsyncEchoServiceProxy : public EchoServiceBase
{
public:
    AsyncEchoServiceProxy() = default;
    virtual ~AsyncEchoServiceProxy() = default;
    virtual std::string Echo(std::string data) override;
private:
    static void EchoCallback(std::string return_value);
};

std::string AsyncEchoServiceProxy::Echo(std::string data)
{
    CLIENT_CALL_RPC_OneParam_Asynchronously(&AsyncEchoServiceProxy::EchoCallback, data);
    return {};
}

void AsyncEchoServiceProxy::EchoCallback(std::string return_value)
{
    std::cout << "Received " << return_value << std::endl;
}

int main(int argc, char* argv[])
{
    auto&& ClientStub = RpcClient::GetRpcClient({ "127.0.0.1", 8888, "client.log" }); // 获取一个RPC Client对象
    auto EchoPtr = ClientStub->GetProxy<AsyncEchoServiceProxy>();// 获取一个RPC Proxy对象
    EchoPtr->Echo("hello world!"); // 发起调用
    sleep(1); // 等一段时间等待返回值到达
    return 0;
}
```

可以看到我们在Proxy中增加了一段回调处理代码，用于处理从服务端到达的返回值。运行上面的客户端，可以看到客户端打印了"Received Server says: hello world!"。

### 同步调用

我们很少使用同步调用，因为它会阻塞线程的执行，进而影响机器性能。但如果你追求语法的简洁，希望Proxy的返回值就是服务器的返回值，实现起来非常简单，不过你可能需要一些额外的工具帮助你阻塞掉当前的线程，比如管道或者future。

这里给出一种使用future实现同步调用的写法。

```cpp
class SyncEchoServiceProxy : public EchoServiceBase
{
public:
    SyncEchoServiceProxy() = default;
    virtual ~SyncEchoServiceProxy() = default;
    virtual std::string Echo(std::string data) override;
};

std::string SyncEchoServiceProxy::Echo(std::string data)
{
    std::promise<std::string> p;
    std::future<std::string> f = p.get_future();
    std::function<void(std::string)> cb = [&p](std::string s) {
        p.set_value(s);
    };
    CLIENT_CALL_RPC_OneParam_Asynchronously(cb, data);
    return f.get();
}

int main(int argc, char* argv[])
{
    auto&& ClientStub = RpcClient::GetRpcClient({ "127.0.0.1", 8888, "client.log" });
    auto EchoPtr = ClientStub->GetProxy<SyncEchoServiceProxy>();
    std::string ret = EchoPtr->Echo("hello world!");
    std::cout << ret << std::endl;
    return 0;
}
```

运行上面的客户端，客户端会打印"Server says: hello world!"，而且是main线程打印，而非异步线程通过回调打印。

### 拓展

服务端的Handle方法的格式是固定的，完全可以交给代码生成工具实现；后续会考虑为RPC添加配套的预处理工具。
