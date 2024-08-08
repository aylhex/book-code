#import <dlfcn.h>
#import <UIKit/UIKit.h>
#import "LoadFishHook.h" 
#import "fishhook.h"
@implementation LoadFishHook 
static int (*orig_close)(int);
static int (*orig_open)(const char *, int, ...);
int my_close(int fd) {
    printf("Calling real close(%d)\n", fd);
    return orig_close(fd);
}
int my_open(const char *path, int oflag, ...) {
    va_list ap = {0};
    mode_t mode = 0;
    if ((oflag & O_CREAT) != 0) {
        va_start(ap, oflag);
        mode = va_arg(ap, int);
        va_end(ap);
        NSLog(@"Calling real open('%s', %d, %d)\n", path, oflag, mode);
        return orig_open(path, oflag, mode);
    }else {
        NSLog(@"Calling real open('%s', %d)\n", path, oflag);
        return orig_open(path, oflag, mode);
    }
}
+ (void)load 
{
    // 利用fishhook提供的rebind_symbols函数实现函数替换
    rebind_symbols((struct rebinding[2]){{"close", my_close, (void *)&orig_close}, {"open", my_open, (void *)&orig_open}}, 2);
    NSLog(@"This func has been hooked by fishhook!!!");
} 
@end