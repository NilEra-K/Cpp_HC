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
- `time_t` : `time_t` 用于表示一个时间类型，它是一个 `long` 类型的别名，在 **<time.h>** 文件中定义，表示从 1970 年 01 月 01日 00 时 00 分 00 秒到现在的秒数，其定义为 `typedef long time_t`
- `time()` 库函数
  - 包含头文件 : `<time.h>`
  - 声明 : `time_t time(time_t* t_loc);`
  - 调用 :
    ```cpp
    // 调用方式 1
    time_t now = time(0);   // 将空地址传递给 time() 函数, 并将 time() 的返回值赋给变量 now

    // 调用方式 2
    time_t now;
    time(&now); // 将变量 now 的地址作为参数传递给 time() 函数
    ```
- `tm` 结构体
  - `time_t` 是一个长整数，不符合人类的使用习惯，需要转换成 `tm` 结构体，`tm`结构体在 `<time.h>` 中声明，如下:
  ```cpp
  struct tm {
      int tm year;  // 年份 : 其值等于实际年份减去 1900
      int tm_mon;   // 月份 : 取值区间为[0, 11], 其中 0 代表一月, 11代表 12月
      int tm_mday;  // 日期 : 一个月中的日期, 取值区间为[1, 31]
      int tm_hour;  // 时:取值区间为[0, 23]
      int tm_min;   // 分:取值区间为[0,59]int tm min;
      int tm_sec;   // 秒:取值区间为[0,59]
      int tm_wday;  // 星期:取值区间为[0,6]，其中0代表星期天，6 代表星期六
      int tm_yday;  // 从每年的 01月 01日开始算起的天数, 取值区间为[0, 365]
      int tm_isdst; // 夏令时标识符，该字段意义不大
  };
  ```
- `localtime()` 库函数
  - `localtime()` 函数用于将 `time_t` 表示的时间转换为 `tm` 结构体表示的时间
  - `localtime()` 函数不是线程安全的，`localtime_r()` 是线程安全的
  - 函数声明
    ```cpp
    struct tm* localtime(const time_t* ttimep);
    struct tm* localtime_r(const time_t* ttimep, struct tm* result);
    ``` 
  - 使用方式
    ```cpp
    #include <iostream>
    #include <time.h>

    using namespace std;

    int main(void) {
        time_t now = time(0);       // 获取当前时间, 存放在 now 中
        cout << "NOW : " << now << endl;

        tm tmnow;
        localtime_r(&now, &tmnow);

        string stime = to_string(tmnow.tm_year + 1900) + "-"
                    + to_string(tmnow.tm_mon + 1) + "-"
                    + to_string(tmnow.tm_mday) + " "
                    + to_string(tmnow.tm_hour) + ":"
                    + to_string(tmnow.tm_min) + ":"
                    + to_string(tmnow.tm_sec);
        cout << "stime = " << stime << endl;
        return 0;
    }
    ```
- `mktime()` 库函数
  - `mktime()` 函数的功能与 `localtime()` 函数相反，用于把 `tm` 结构体时间转换为 `time_t` 时间
  - 包含头文件 : `<time.h>`
  - 函数声明 : `time_t mktime(struct tm* tm)`
  - 该函数主要用于时间的运算，例如 : 把 `2022-10-01 15:30:25` 加增加 30 分钟
    - 思路: <p>
      ① 解析字符串格式的时间，转换成 `tm` 结构体 <p>
      ② 用 `mktime()` 函数将 `tm` 结构体转换成 `time_t` 时间 <p>
      ③ 把 `time_t` 时间加 30 * 60 秒 <p>
      ④ 把 `time_t` 时间转换成 tm 结构体 <p>
      ⑤ 将 `tm` 结构体转换成字符串
- `gettimeofday()` 库函数
  - 用于获取 1970 年 01 月 01 日到现在的秒和当前秒中也已经逝去的微秒数，可以用于程序的计时
  - 包含头文件 : `<sys/time.h>`
  - 函数声明
    ```cpp
    #include <sys/time.h>
    int gettimeofday(struct timeval* tv, struct timezone* tz);

    struct timeval {
        time_t        tv_sec;   // 1970-01-01 到现在的秒数 
        susecounds_t  tv_usec;  // 在当前秒中已经逝去的微秒数
    };

    struct timezone { // 在实际开发中, 派不上用场
        int tz_minuteswest;
        int tz_dsttime;
    };
    ```
  - 使用方式
    ```cpp
    #include <iostream>
    #include <sys/time.h>

    int main(void) {
        timeval start, end;
        gettimeofday(&start, 0);     // 计时开始
        for(int i = 0; i < 10000; i++) { }
        gettimeofday(&end, 0);       // 计时结束

        // 计算消耗的时长
        timeval tv;
        tv.tv_usec = end.tv_usec - start.tv_usec;
        tv.tv_sec  = end.tv_sec  - end.tv_sec;
        if(tv.tv_usec < 0) {
            tv.tv_usec = 1000000 - tv.tv_usec;
            tv.tv_sec--;
        }
        cout << "耗时 : " << tv.tv_sec << " 秒和 : " << tv.tv_usec << "微秒" << endl; 
    }
    ```
- 程序睡眠
  - 使用 `sleep()` 或 `usleep()` 函数可以将程序挂起一段时间
  - 函数声明
    ```cpp
    unsigned int sleep(unsigned int seconds);
    int usleep(useconds_t usec);
    ```
### 8. Linux 目录操作
- 获取当前工作目录
  - 包含头文件 : `<unistd.h>`
  - 函数声明
    ```cpp
    char* getcwd(char* buf, size_t size);
    char* get_current_dir_name(void);
    ```
  - 使用方式
    ```cpp
    #include <iostream>
    #include <unistd.h>

    using namespace std;

    int main(void) {
        char path1[256];
        getcwd(path1, 256);     // Linux 系统目录的最大长度为 255
        cout << "path1 = " << path1 << endl;

        char* path2 = get_current_dir_name();
        cout << "path2 = " << path2 << endl;
        free(path2);            // 注意释放内存
    }
    ```
- 切换工作目录
  - 包含头文件 : `<unistd.h>`
  - 函数声明 : `int chdir(const char* path);`
  - 返回值 : `0` 成功；`其他` 失败(目录不存在或没有权限)
- 创建目录
  - 包含头文件 : `<sys/stat.h>`
  - 函数声明 : `int mkdir(const char* pathname, mode_t mode);` <p>
    `pathname` : 目录名
    `mode` : 访问权限，如 *0755*
  - 返回值 : `0` 成功；`其他` 失败(上级目录不存在或没有权限)
- 删除目录
  - 包含头文件 : `<unistd.h>`
  - 函数声明 : `int rmdir(const char* path);`
  - 返回值 : `0` 成功；`其他` 失败(目录不存在或没有权限)
- 获取目录中文件的列表
  - 文件存放在目录中，在处理文件之前，必须先知道目录中有那些文件，所以要获取目录中的文件列表
  - 包含头文件 : `<dirent.h>`
  - 相关库函数(同时也是步骤)
    - 使用 `opendir()` 函数打开目录 <p>
      `DIR* opendir(const char* pathname);`<p>
      成功返回目录的地址，失败返回空地址
    - 使用 `readdir()` 函数读取目录 <p>
      `struct dirent* readdir(DIR* dirp);` <p>
      成功返回`struct dirent`结构体的地址，失败返回空地址
    - 使用 `closedir()` 关闭目录 <p>
      `int closedir(DIR* dirp);` <p>
- 相关数据结构
  - 目录指针 : `DIR* 目录指针变量名`
  - 每次调用 `readdir()`，函数返回`struct dirent`的地址，存放了本次读取到的内容。
    ```cpp
    struct dirent {
        // inode number 索引节点号
        long  d_ino;

        // offset to this dirent 在目录文件中的偏移
        off_t d_off;
        
        // length of this d_name 文件名长度
        unsigned short d_reclen;

        // the type of d_name 文件类型
        // 有多种取值, 最重要的是 8 和 4
        // 8 - 常规文件
        // 4 - 目录文件
        unsigned char d_type;

        // file name 文件名, 最长不超过 255 字符
        char d_name[NAME_MAX + 1]；
    };
    ```
### 9. Linux 系统错误
- 在 C++ 程序中，如果调用了库函数，可以通过函数的返回值判断调用是否成功。其实，还有一个整型的全局变量 `errno`，存放了函数调用过程中产生的错误代码。<p> 如果调用库函数失败，可以通过 `errno` 的值来查找原因，这也是调试程序的一个重要方法。<p> `errno` 在 `<errno.h>` 中声明，配合 `strerror()` 和 `perror()` 两个库函数，可以查看出错的详细信息。
- `strerror()` 库函数
  - `strerror()` 在 `<string.h>` 头文件中声明，用于获取错误代码对应的详细信息。
  - 函数声明
    ```cpp
    char* strerror(int errnum);                           // 线程安全
    int strerror_r(int errnum, char* buf, size_t buflen); // 非线程安全
    ```
    *gcc 8.3.1* 共有 133 个错误代码
  - 使用方式
    ```cpp
    #include <iostream>
    #include <cstring>

    using namespace std;

    int main(void) {
        for(int i = 0; i < 150; i++) {
            cout << i << " : " << strerror(i) << endl;
        }
        return 0;
    }
    ```
  - 注意事项
    - 调用库失败不一定会设置 `errno` <p>
      并不是全部的库函数在调用失败时都会设置 `errno` 值，以 `man` 手册为准(不属于系统调用的函数不会设置 `errno`)，属于系统调用的函数才会设置 `errno`
    - `errno` 不能作为调用库函数失败的标志 <p>
      `errno` 的值只有在库函数调用发生错误时才会被设置，当库函数调用成功时，`errno`的值不会被修改，不会主动的置为 0
      ```cpp
      #include <iostream>
      #include <cstring>      // strerror() 函数需要的头文件
      #include <cerrno>       // errno 全局变量需要的头文件
      #include <sys/stat.h>   // mkdir() 函数需要的头文件

      using namespace std;

      int main(void) {
          // 实际失败, 更新 errno
          int iret = mkdir("./tmp/aa/bb/cc", 0775);    
          cout << "iret: " << iret << endl;
          cout << errno << " : " << strerror(errno) << endl;
          perror("调用 mkdir(\"./tmp/aa/bb/cc\", 0775) 失败");

          // 实际成功, 但是没有更新 errno
          iret = mkdir("./tmp", 0775);
          cout << "iret: " << iret << endl;
          cout << errno << " : " << strerror(errno) << endl;
          perror("调用 mkdir(\"./tmp\", 0775) 失败");

          return 0;
      }
      ```
      执行程序如下结果如下：
      ```text
      iret: -1
      2 : No such file or directory
      调用 mkdir("./tmp/aa/bb/cc", 0775) 失败: No such file or directory
      iret: 0
      2 : No such file or directory
      调用 mkdir("./tmp", 0775) 失败: No such file or directory
      ```
      修改后的代码如下:
      ```cpp
      #include <iostream>
      #include <cstring>      // strerror() 函数需要的头文件
      #include <cerrno>       // errno 全局变量需要的头文件
      #include <sys/stat.h>   // mkdir() 函数需要的头文件

      using namespace std;

      int main(void) {
          // 实际失败, 更新 errno
          int iret = mkdir("./tmp/aa/bb/cc", 0775);    
          if(iret != 0) {
              cout << "iret: " << iret << endl;
              cout << errno << " : " << strerror(errno) << endl;
              perror("调用 mkdir(\"./tmp/aa/bb/cc\", 0775) 失败");
          }

          // 实际成功, 但是没有更新 errno
          iret = mkdir("./tmp", 0775);
          if(iret != 0) {
              cout << "iret: " << iret << endl;
              cout << errno << " : " << strerror(errno) << endl;
              perror("调用 mkdir(\"./tmp\", 0775) 失败");
          }
          return 0;
      }
      ```
      修改后输出如下：
      ```text
      iret: -1
      2 : No such file or directory
      调用 mkdir("./tmp/aa/bb/cc", 0775) 失败: No such file or directory
      ```
### 10. Linux目录和文件的操作
- `access()` 库函数
  - 包含头文件：`#include <unistd.h>`
  - 函数声明：`int access(const char* pathname, int mode)` <p>
  - 参数说明：<p>
    `pathname`：目录或文件名
    `mode`：需要判断的存取权限。在头文件`<unistd>`中的预定义如下：
    ```cpp
    #define R_OK  4
    #define W_OK  2
    #define X_OK  1
    #define F_OK  0
    ```
  - 返回值：<p>
    当 `pathname` 满足 `mode` 权限返回 `0`，不满足返回 `-1`，`errno` 被设置 <p>
    在实际开发中，`access()` 函数主要用于判断目录或者文件是否存在
- `stat()` 库函数
  - `stat` 结构体
    ```cpp
    struct stat  
    {   
        dev_t       st_dev;     /* ID of device containing file -文件所在设备的ID*/  
        ino_t       st_ino;     /* inode number -inode节点号*/    
        mode_t      st_mode;    /* protection -保护模式?*/    
        nlink_t     st_nlink;   /* number of hard links -链向此文件的连接数(硬连接)*/    
        uid_t       st_uid;     /* user ID of owner -user id*/    
        gid_t       st_gid;     /* group ID of owner - group id*/    
        dev_t       st_rdev;    /* device ID (if special file) -设备号，针对设备文件*/    
        off_t       st_size;    /* total size, in bytes -文件大小，字节为单位*/    
        blksize_t   st_blksize; /* blocksize for filesystem I/O -系统块的大小*/    
        blkcnt_t    st_blocks;  /* number of blocks allocated -文件所占块数*/    
        time_t      st_atime;   /* time of last access -最近存取时间*/    
        time_t      st_mtime;   /* time of last modification -最近修改时间*/    
        time_t      st_ctime;   /* time of last status change - */    
    };  
    ```
    struct stat结构体的成员变量比较多，重点关注 `st_mode`、`st_size`和`st_mtime`成员，注意: `st_mtime` 是一个正数表示的时间，需要程序员自己写代码转换格式 <p>
    `st_mode` 成员的取值很多，或者使用如下两个宏来判断。
    - `S_ISREG(st_mode)` 是否为普通文件，如果是返回真
    - `S_ISDIR(st_mode)` 是否为目录，如果是返回真
  - `stat()` 库函数
    - 包含头文件：`#include <sys/stat.h>`
    - 函数声明：`int stat(const char* path, struct stat* buf)`
    - 返回值：`0`-成功；`-1`失败，`errno`被设置
    - `stat()` 函数获取 `path` 参数指定目录或文件的详细信息，保存到 `buf` 结构体中。
    - 示例代码
      ```cpp
      #include <iostream>
      #include <cstdio>
      #include <cstring>
      #include <sys/stat.h>
      #include <unistd.h>

      using namespace std;
      int main(int argc, char* argv[]) {
          if(argc != 2) {
              cout << "[USAGE] ./dir_test <filename | dirname>" << endl;
              return -1;
          }
          struct stat st; // 存放目录或者文件详细信息的结构体

          // 获取目录或文件的详细信息
          if (stat(argv[1], &st) != 0) {
              cout << "stat(" << argv[1] << "):" << strerror(errno) << endl;
          }

          if(S_ISREG(st.st_mode)) {
              cout << argv[1] 
                  << "是一个文件(" 
                  << "mtime = "
                  << st.st_mode
                  << ", size = "
                  << st.st_size
                  << ")" << endl;
          }

          if(S_ISDIR(st.st_mode)) {
              cout << argv[1] 
                  << "是一个目录(" 
                  << "mtime = "
                  << st.st_mode
                  << ", size = "
                  << st.st_size
                  << ")" << endl;
          }
      }
      ```
  - `utime()` 库函数
    - 包含头文件：`#include <utime.h>`
    - 函数声明：`int utime(const char* filename, const struct utimebuf* times);`
    - `utimebuf` 结构声明如下：
      ```cpp
      struct utimbuf {
          time_t actime;
          time_t modtime;
      };
      ```
    - 返回值：`0`-成功；`-1`-失败，`errno` 被设置
    - `utime()` 函数用来修改函数 `filename` 的 `st_atime` 和 `st_mtime`。如果参数 `times` 为空地址，则设置为当前时间。
  - `rename()` 库函数
    - 包含头文件：`#include <stdio.h>`
    - 函数声明：`int rename(const char* oldpath, const char* newpath)`
    - 返回值：`0`-成功；`-1`-失败，`errno` 被设置







