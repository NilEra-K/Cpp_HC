// gdb 调试正在运行中的程序
#include <iostream>
#include <unistd.h>

using namespace std;

void func_1(int x, int y) {
    for(int i = 0; i < 1000; i++) {
        sleep(1);
        cout << "i = " << i << " x = " << x << " y = " << y <<endl;
    }
}

void func_2(void) {
    func_1(10, 20);
}

int main(void) {
    func_2();
}