//
//  main.m
//  antidebug
//
//  Created by AYL on 2023/1/5.
//

#import <UIKit/UIKit.h>
#import "AppDelegate.h"
#import <dlfcn.h>
#import <sys/types.h>
#import <sys/syscall.h>
#import <sys/sysctl.h>
#include <unistd.h>
#include <sys/ioctl.h>

#ifndef PT_DENY_ATTACH
#define PT_DENY_ATTACH 31
#endif
typedef int (*ptrace_ptr)(int _request, pid_t _pid, caddr_t _addr, int _data);
 
void check_debug_by_ptrace(){
    void* handle = dlopen(0, RTLD_GLOBAL | RTLD_NOW);
    ptrace_ptr ptrace = dlsym(handle, "ptrace");
    ptrace(PT_DENY_ATTACH, 0, 0, 0);
    dlclose(handle);
}
void check_debug_by_syscall(){
    // SYS_ptrace 26
    syscall(26,PT_DENY_ATTACH,0,0);
}

void check_debug_by_svc() {
#ifdef __arm64__
    __asm__("mov X0, #31\n"
            "mov X1, #0\n"
            "mov X2, #0\n"
            "mov X3, #0\n"
            "mov w16, #26\n"  // 26是ptrace的系统调用号
            "svc #0x80");
#endif
}

// 等同syscall(SYS_ptrace, PT_DENY_ATTACH, 0, 0, 0)
void check_debug_by_svc_syscall() {
#ifdef __arm__
    __asm__("mov r0, #26\n"
            "mov r1, #31\n"
            "mov r2, #0\n"
            "mov r3, #0\n"
            "mov r12, #16\n"
            "svc #80");
#endif
#ifdef __arm64__
    __asm__("mov X0, #26\n"
            "mov X1, #31\n"
            "mov X2, #0\n"
            "mov X3, #0\n"
            "mov X4, #0\n"
            "mov w16, #0\n"
            "svc #128");
#endif
}

void check_debug_by_sysctl(){
    // 需要检测进程信息的字段数组
    int proc[4];
    proc[0] = CTL_KERN;
    proc[1] = KERN_PROC;
    proc[2] = KERN_PROC_PID;
    proc[3] = getpid();
    // 查询进程信息结果的结构体
    struct kinfo_proc info;
    size_t info_size = sizeof(info);
    info.kp_proc.p_flag = 0;

    int ret = sysctl(proc, sizeof(proc)/sizeof(*proc), &info, &info_size, NULL, 0);
    if (ret == -1) {
        return;
    }
    // 根据标记位检测调试状态
    if((info.kp_proc.p_flag & P_TRACED) == 0)
    {
        exit(0);
    }
}


void check_debug_by_isatty() {
  if (isatty(STDIN_FILENO)) {
    exit(1);
  }
}


void check_debug_by_ioctl() {
  if (ioctl(PT_DENY_ATTACH, 0) == -1) {
    exit(1);
  }
}

void check_debug(){
//    check_debug_by_ptrace();
//    check_debug_by_syscall();
//    check_debug_by_svc();
//    check_debug_by_sysctl();
//    check_debug_by_isatty();
    
}

int main(int argc, char * argv[]) {
    
//    while (true) {
//        NSLog(@"Check Debug");
//        check_debug();
//    }
    NSString *appPath = [[NSBundle mainBundle] executablePath];
    NSLog(@"appPath %@\n",appPath);
    
    NSString * appDelegateClassName;
    @autoreleasepool {
        // Setup code that might create autoreleased objects goes here.
        appDelegateClassName = NSStringFromClass([AppDelegate class]);
        
    }
    return UIApplicationMain(argc, argv, nil, appDelegateClassName);
}
