#include <iostream>
#include <sys/time.h>
#include <time.h>

using namespace std;

int main(void) {
    cout << "=====     time()     =====" << endl;
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

    cout << "===== gettimeofday() =====" << endl;
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
    return 0;
}
