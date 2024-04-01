# Cpp_HC
C++ Highly Concurrent

学习 Linux 下 C++ 的高并发

- **Linux 环境高级编程 :** 了解 Linux 编程常用知识，为学习网络编程和项目实战做准备，在实际开发中，90%的C++程序运行在 Linux 系统中
- **C++ 网络编程基础 :** 了解网络编程的基础知识，为学习高并发的 Reactor 服务器做准备
  - socket 基础
  - socket 库函数
  - IO 复用 (select、poll、epoll)
  - 阻塞/非阻塞IO
- **实现 Reactor 高并发服务器 (仿 muduo)**
  - Reactor模型是一种高效的网络设计模式，它在处理并发连接时表现出色，适合构建高性能、可伸缩的网络服务器
  - redis、memcache、nginx、rpc框架都是基于 Reactor 模式来实现的


## Linux 环境高级编程
### 1. man 手册的使用
  - `man <level> <command/function>`
  - man的级别:
    - `1` 用户命令
    - `2` 系统接口
    - `3` 库函数
    - `4` 特殊文件，例如设备文件
    - `5` 文件
    - `6` 游戏
    - `7` 系统的软件包
    - `8` 系统管理命令
    - `9` 内核
### 2. gcc/g++ 编译操作
  - `gcc/g++ [files] <option>`
  - 选项说明
    - `-o` : 指定输出的文件名，该文件名不可以与源文件同名，如果不给出这个选项，则生成可执行文件 `a.out`
    - `-g` : 如果想对源代码进行调试，需要加入这个选项
    - `-c` : 只编译，不链接成为可执行文件，通常把源文件便已成静态库或动态库
    - `-On` : 在编译、链接过程中进行优化处理，生成的可执行程序效率将更高
      - `-O0` : 不做任何优化
      - `-O/-O1` : 对程序做部分编译优化，对于大函数，优化编译占用稍微多的时间和相当大的内存，使用本项优化，编译器会尝试减小代码的尺寸，以及缩短执行时间，但并不执行需要占用大量编译时间的优化
      - `-O2` : **(推荐的优化等级)** 是比 `-O1` 更高级的选项，进行更多的优化，与 `-O1` 比较而言，`-O2` 优化增加了编译时间的基础上，提高了生成代码的执行效率
      - `-O3` : **(危险的优化等级)** 比 `O2` 更进一步的优化，用这个选项会延长编译代码的时间，并且在使用 *gcc 4.x* 的系统里不应全局启用，自 *gcc 3.x* 版本以来，gcc 的行为已经有了极大的改变。在 *gcc 3.x* ，`-O3` 生成的代码也只是比 `-O2` 快一点点，而在 `gcc 4.x` 中还未必更快。用 `-O3` 来编译的所有软件包将产生更大体积、更耗内存的二进制文件，大大增加编译失败的机会或不可与之的程序行为(包括错误)。
      - 如果使用了优化选项 : 1> 编译时间将更长; 2> 目标程序不可调试; 3> 有效果，但是不可能显著提升程序的性能。
    - `-std=c++11` 支持 C++ 11 标准
### 3. 静态库和动态库
  - **静态库**
    - 什么是静态库 : 程序在编译时会把库文件的二进制代码链接到目标程序中，这种方式成为静态链接
    - 如何制作静态库 : 
      ```bash
      # 第一种方式: 直接生成静态库文件
      g++ -c <files> -o libName.a
      
      # 第二种方式: 先生成 .o 文件，再生成库文件
      g++ -c <files>
      ar -r libName.a <.o Files>      
      ```
    - 如何使用静态库 : <p>
      `g++ <files> <options> -l<Name> -L<libPath>` <p>
      其中 `options` 表示选项，`<Name>` 表示库名称，`<libPath>` 表示库路径
    - 静态库的特点 :
      - 静态库的链接是在编译时期完成的，执行的时候代码加载速度快
      - 因为程序在编译时会把库文件的二进制代码链接到目标程序中，因此目标程序的可执行文件比较大，浪费空间
      - 程序的更新和发布不方便，如果一个静态库更新了，所有使用它的程序都需要重新编译
  - **动态库**
    - 什么是动态库 : 程序在编译时不会把库文件的二进制代码链接到目标程序中，而是在循行时才被载入，如果多个程序中用到了同一动态库中的函数，那么在内存中只有一份，避免了空间的浪费问题
    - 如何制作动态库 :
      `g++ -fPIC -shared <files> -o libName.so`
    - 如何使用动态库 :
      `g++ <files> <options> -l<Name> -L<libPath>` <p>
      其中 `options` 表示选项，`<Name>` 表示库名称，`<libPath>` 表示库路径 <p>
      **[ 注意 ]** 在 CentOS 7 (Ubuntu 22.04) 运行可执行程序的时候，需要提前设置 `LD_LIBRARY_PATH` 环境变量，添加动态库所在位置，否则将会出现错误 : `ERROR While Loading Shared Libraries: libName.so : Cannot Open Shared Object File: No Such File or Directory` <p>
      可以使用 `echo $LD_LIBRARY_PATH` 来查看当前的环境变量 <p>
      可以使用 `export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:<New_Path>` 来添加新的环境变量
    - 动态库的优点 :
      - 程序运行再运行的过程中，需要用到动态库的时候才把动态库的二进制代码载入内存
      - 可以实现进程之间的代码共享，因此动态库也成为共享库
      - 程序升级比较简单，不需要重新编译程序，只需要更新动态库即可
  - 如果静态库和动态库同时存在，编译器将优先使用动态库
---
### 4. Makefile
- 在实际开发中，项目源代码文件比较多，按类型、功能、模块分别存放在不同的目录和文件中，哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，还有其他复杂的操作...
- `make` 是一个强大的实用工具，用于管理项目的编译和链接
- `make` 需要一个编译规则文件 `makefile` ，可实现自动化编译
---
### 5. `main` 函数的参数
- `main` 函数有三个参数，`argc`、`argv`和`envp`，它的标准写法为 : 
  ```cpp
  int main(int argc, char* argv[], char* envp[]) {
      // argc : 存放了程序参数的个数, 包括程序本身
      // argv : 字符串数组, 存放了每个参数的值, 包括程序本身
      // envp : 字符串数组, 存放了环境变量, 数组的最后一个元素为空
      return 0;
  }
  ```
- 在程序中，如果不关心 `main` 函数的参数，可以省略不写
- 操作环境变量
  - 设置环境变量
    ```cpp
    int setenv(const char* name, const char* value, int overwrite);
    // name       环境变量名
    // value      环境变量值
    // overwrite  0   - 如果环境不存在, 增加新的环境变量; 如果环境已经存在, 不替换该值
    //            非0 - 如果环境不存在, 增加新的环境变量; 如果环境已经存在, 替换该值
    // return     0 - 成功
    //            1 - 失败
    // 此函数设置的环境变量只对本进程有效, 不会影响 shell 的环境变量
    // 如果在运行程序的时候执行了 setenv() 函数, 进程终止后再次运行该程序, 上次的设置是无效的
    ```
  - 获取环境变量的值
    ```cpp
    char* getenv(const char* name);
    ```
### 6. gdb 调试
- 如果希望程序可以调试，编译时需要增加 `-g` 选项，并且不能使用 `-O` 的优化选项
- 使用 `gdb <target_program>` 来运行 gdb
- gdb 的常用命令
  | 命令 | 简写 | 命令说明 |
  | :-: | :-: | :-: |
  | `set args` | - | 设置主程序的参数 <p> `set args <param1> <param2> ...` |
  | `break` | `b` | 设置断点，`b <n>` 表示在第 `n` 行设置断点 <p> 可以设置多个断点 |
  | `run` | `r` |  开始运行程序，程序运行到断点的位置会停下来，如果没有遇到断点，程序会一直运行下去 |
  | `next` | `n` | 执行当前行语句，如果该语句为函数调用，不会进入函数内部 |
  | `step` | `s` | 执行当前行语句，如果该语句为函数调用，则会进入函数内部 <p> 如果函数是库函数或第三方提供的函数 **(没有源代码)**，用 `s` 也无法进入函数内部 |
  | `print` | `p`| 显示变量或表达式的值，如果 `p` 后面是表达式，则会执行这个表达式 |
  | `continue` | `c` | 继续运行程序，遇到下一个断点停止，如果没有遇到断点，程序将一直运行 |
  | `set var` | - | 设置变量的值 |
  | `quit` | `q` | 退出 gdb |
  | `backtrace` | `bt` | 函数调用栈，调试 *core* 文件时常用 |
- gdb 调试 core 文件
  - 如果程序在运行的过程中发生了内存泄漏，会被内核强行终止，提示*段错误(吐核)*，内存的状态将保存在 *core* 文件中，方便程序员进一步分析
  - Linux缺省你不会生成 *core* 文件，需要修改系统参数
  - 调试 *core* 文件的步骤如下:
    - 使用 `ulimit -a` 查看当前用户的资源限制参数
      ```bash
      nilera@nilera-virtual-machine:~$ ulimit -a
      real-time non-blocking time  (microseconds, -R) unlimited
      core file size              (blocks, -c) 0
      data seg size               (kbytes, -d) unlimited
      scheduling priority                 (-e) 0
      file size                   (blocks, -f) unlimited
      pending signals                     (-i) 15183
      max locked memory           (kbytes, -l) 495440
      max memory size             (kbytes, -m) unlimited
      open files                          (-n) 1048576
      pipe size                (512 bytes, -p) 8
      POSIX message queues         (bytes, -q) 819200
      real-time priority                  (-r) 0
      stack size                  (kbytes, -s) 8192
      cpu time                   (seconds, -t) unlimited
      max user processes                  (-u) 15183
      virtual memory              (kbytes, -v) unlimited
      file locks                          (-x) unlimited
      ```
    - 使用 `ulimit -c unlimited` 把 `core file size` 改为 `unlimited`
    - 运行程序，产生 *core* 文件，如果没有产生 *core* 文件，需要使用 `sudo service apport stop` 命令，防止 *core* 被Linux的 `apport` 进程吞掉，检查是否为系统 Bug
    - 如果程序发生段错误，可以使用 `bt` 查看**函数调用栈** ，来观察函数的调用顺序
- gdb 调试正在运行中的程序
  - 要想调试正在运行中的程序，需要得到程序的进程 id
  - 使用 `ps -ef | grep <program_name>` 查看进程id
    ```bash
    nilera@nilera-virtual-machine:~$ ps -ef | grep debug_run
    nilera      9968    5227  0 18:42 pts/3    00:00:00 ./debug_run
    nilera     10549   10022  0 18:44 pts/4    00:00:00 grep --color=auto debug_run
    ```
  - 使用 `gdb debug_run -p 9968` 对正在运行中的程序进行调试
  - 调试正在运行中的程序时，正在运行中的程序会暂停，随着 `gdb` 的命令进行操作

### 7. Linux 时间操作
UNIX 操作系统根据计算机产生的年代，把 1970 年 01 月 01日作为 UNIX 的纪元时间，将从 1970 年 01 月 01 日起经过的秒数用一个整数存放



