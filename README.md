# Cpp_HC
C++ Highly Concurrent

学习 Linux 下 C++ 的高并发

- **Linux 环境高级编程 :** 了解 Linux 编程常用知识, 为学习网络编程和项目实战做准备, 在实际开发中, 90%的C++程序运行在 Linux 系统中
- **C++ 网络编程基础 :** 了解网络编程的基础知识, 为学习高并发的 Reactor 服务器做准备
  - socket 基础
  - socket 库函数
  - IO 复用 (select、poll、epoll)
  - 阻塞/非阻塞IO
- 实现 Reactor 高并发服务器 (仿 muduo)
  - Reactor模型是一种高效的网络设计模式, 它在处理并发连接时表现出色, 适合构建高性能、可伸缩的网络服务器
  - redis、memcache、nginx、rpc框架都是基于 Reactor 模式来实现的

---
### Linux 环境高级编程
man 手册的使用: `man <level> <command/function>`
- man的级别:
  - `1` 用户命令
  - `2` 系统接口
  - `3` 库函数
  - `4` 特殊文件, 例如设备文件
  - `5` 文件
  - `6` 游戏
  - `7` 系统的软件包
  - `8` 系统管理命令
  - `9` 内核

