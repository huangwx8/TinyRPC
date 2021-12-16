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
