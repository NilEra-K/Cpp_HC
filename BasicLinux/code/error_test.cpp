#include <iostream>
#include <cstring>

using namespace std;

int main(void) {
    for(int i = 0; i < 150; i++) {
        cout << i << " : " << strerror(i) << endl;
    }
    return 0;
}