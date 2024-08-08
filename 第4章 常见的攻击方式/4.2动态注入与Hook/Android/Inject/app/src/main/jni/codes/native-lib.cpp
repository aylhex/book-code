//
// Created by AYL on 2022/11/24.
//
#include <dlfcn.h>
#include <android/log.h>
#include <cstring>

#define C_API extern "C"
#define PUBLIC_METHOD __attribute__ ((visibility ("default")))

#define LOGD(format, ...) __android_log_print(ANDROID_LOG_DEBUG, "PreloadTest", format, ##__VA_ARGS__);

C_API PUBLIC_METHOD int access(const char *pathname, int mode) {
    static int (*orig_access)(const char *pathname, int mode) = nullptr;
    if (orig_access == nullptr) {
        void *libc = dlopen("libc.so", 0);
        if (libc == nullptr) {
            return 0;
        }
        orig_access = reinterpret_cast<int (*)(const char *, int)>(dlsym(libc, "access"));
        if (orig_access == nullptr) {
            return 0;
        }
    }
    // 避免陷入死循环,高版本调用__android_log_print会访问access函数请求/dev/socket/logdw权限
    // 因此此时再调用__android_log_print则会陷入死循环
    if (strcmp("/dev/socket/logdw", pathname) == 0){
        return orig_access(pathname, mode);
    }
    LOGD("Hook模块监听access函数,路径: %s, mode: %d", pathname, mode);
    if (strcmp("/data/local/tmp/inject-test", pathname) == 0){
        // 如果是我们测试文件访问则返回-1没有该文件
        return -1;
    }
    return orig_access(pathname, mode);
}

static __attribute__((constructor)) void Init() {
    LOGD("LD_PRELOAD模块被加载");
}
