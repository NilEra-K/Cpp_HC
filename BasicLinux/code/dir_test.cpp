#include <iostream>
#include <unistd.h>
#include <dirent.h>

using namespace std;

#if 0
int main(void) {
    char path1[256];
    getcwd(path1, 256);     // Linux 系统目录的最大长度为 255
    cout << "path1 = " << path1 << endl;

    char* path2 = get_current_dir_name();
    cout << "path2 = " << path2 << endl;
    free(path2);            // 注意释放内存
    return 0;
}
#endif

#if 1
int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "[Usage] ./dir_test <dir_path>" << endl;
        return -1;
    }

    DIR* dir;       // 定义目录指针
    if((dir = opendir(argv[1])) == 0) { // 打开目录
        return -1;
    }

    struct dirent* stdinfo = nullptr;
    while(1) {
        if((stdinfo = readdir(dir)) == nullptr) {
            break;
        }
        cout << "File Name : " << stdinfo->d_name << endl 
             << "File Type : " << (int)stdinfo->d_type << endl;
    }

    closedir(dir);  // 关闭目录
    return 0;
}
#endif