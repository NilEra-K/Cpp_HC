#include <iostream>
#include <cstring>      // strerror() 函数需要的头文件
#include <cerrno>       // errno 全局变量需要的头文件
#include <sys/stat.h>   // mkdir() 函数需要的头文件

using namespace std;

int main(void) {
    for(int i = 0; i < 150; i++) {
        cout << i << " : " << strerror(i) << endl;
    }

    // 实际失败, 更新 errno
    int iret = mkdir("./tmp/aa/bb/cc", 0775);    
    if(iret != 0) {
        cout << "iret: " << iret << endl;
        cout << errno << " : " << strerror(errno) << endl;
        perror("调用 mkdir(\"./tmp/aa/bb/cc\", 0775) 失败");
    }

    // 实际失败, 但是没有更新 errno
    iret = mkdir("./tmp", 0775);
    if(iret != 0) {
        cout << "iret: " << iret << endl;
        cout << errno << " : " << strerror(errno) << endl;
        perror("调用 mkdir(\"./tmp\", 0775) 失败");
    }
    return 0;
}