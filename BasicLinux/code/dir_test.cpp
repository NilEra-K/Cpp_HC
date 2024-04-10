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

#if 0
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

#if 0
#include <iostream>
#include <cstdio>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;
int main(int argc, char* argv[]) {
    if(argc != 2) {
        cout << "[USAGE] ./dir_test <filename | dirname>" << endl;
        return -1;
    }

    if(access(argv[1], F_OK) != 0) {
        cout << "文件或目录" << argv[1] << "不存在" << endl;
        return -1;
    }
    cout << "文件或目录" << argv[1] << "已存在" << endl;
    return 0;
}
#endif

#if 1
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
#endif