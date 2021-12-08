# TinyRPC

- by wexuanhuang

## 整体设计

整体模块划分方式参考 trpc-cpp。

序列化协议使用

传输层协议使用TCP。

Runtime层使用Reactor模型，IO复用使用Epoll Edge-Trigger模式。

支持异步远程调用与回调函数，支持异步日志。
