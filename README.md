# TinyRPC

- by wexuanhuang

## 整体设计

整体模块划分方式参考 trpc-cpp。

请求报文包括(seqno, rpcname, rpcparams)，响应报文包括(seqno, retval)。

rpcparams和retval被序列化协议序列化，序列化协议使用线性组合策略。

传输层协议使用TCP，封装Linux底层Socket系统调用。

Runtime层使用带线程池的Reactor模型，IO复用使用Epoll ET模式。

Common层实现了RPC调用接口，全局Uid管理，异步日志，定时器管理等通用功能。

仅支持异步远程调用，支持客户端绑定回调函数。

## 使用方法

这里通过演示一个简单的客户端调用服务器的helloworld的用法，简单介绍本框架的使用方法。

为了实现远端调用，你需要一个Rpc代理类，用于客户端发起(invoke)Rpc调用；同时需要在服务器完成Rpc任务的具体实现(implement)，用于具体地执行任务并发回函数返回值。

### 服务基类

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
    virtual int Echo(const char* Message) = 0;
};
```

### 服务器

继承`EchoServiceBase`类，在服务端实现Rpc调用的具体实现。除了实现函数你的函数，你还需要填补Handle函数

然后，服务器的main内，启动服务器并注册服务；这时服务器就可响应所有到来的Rpc请求了。

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

int main(int argc, char* argv[])
{
    // 服务实现，服务的具体执行由它来完成
    EchoServiceImpl EchoServer;

    // 启动Rpc服务端
    auto&& ServerStub = RpcServer::GetRpcServer({"localhost",8888,"server.log"});
    
    // 实现绑定到Rpc服务端
    ServerStub.RegisterService(&EchoServer);

    // 接受请求
    sleep(3600);
}
```

### 客户端

继承`EchoServiceBase`类，在客户端实现Rpc调用代理，只需在客户端的代理函数内添加一条宏。

启动Rpc客户端，调用客户端方法获取服务代理对象，然后便可愉快地调用Rpc了。

```cpp
class EchoServiceProxy : public EchoServiceBase
{
public:
    virtual int Echo(const char* Message) override
    {
        CLIENT_CALL_RPC_OneParam(Message);
    }
};

int main(int argc, char* argv[])
{
    // 启动Rpc客户端
    auto&& ClientStub = RpcClient::GetRpcClient({"localhost",8888,"client.log"});

    // 获取服务代理
    auto EchoPtr = ClientStub.GetProxy<EchoServiceProxy>();

    // 绑定回调函数
    EchoPtr->SetCallback(
        [](int x) {
            log_dev("Echo returns [%d]\n", x);
        }
    );

    // 发送RPC
    EchoClient.Echo("hello world!");

    // 等待服务器的返回值
    sleep(1);
}
```

### 拓展

本框架依然需要用户去手动编写服务端的Handle方法和客户端的Invoke方法，但实际上这两个方法的代码是固定格式的，完全可以交给代码生成工具，在预处理之前去自动生成。后续如果有空，我会补全这个缺陷。

Rpc报文当前使用结构体，为定长报文。这样的实现无论是效率和可拓展性都不理想，后续会考虑将RpcParams项实现为变长。
