#include <iostream>
#include "myMath.h"

int main(void) {
    std::cout << "Hello World" << std::endl;
    int x = 20, y = 30;
    int ans = 0;
    add(x, y, &ans);
    std::cout << ans << std::endl;
    return 0;
}