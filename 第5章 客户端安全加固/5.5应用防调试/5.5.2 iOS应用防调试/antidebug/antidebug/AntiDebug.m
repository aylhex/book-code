//
//  AntiDebug.m
//  antidebug
//
//  Created by Ann on 2023/1/8.
//

#import "AntiDebug.h"

@implementation AntiDebug

#ifndef PT_DENY_ATTACH
#define PT_DENY_ATTACH 31
#endif
typedef int (*ptrace_ptr)(int _request, pid_t _pid, caddr_t _addr, int _data);
 
-(void) anti_debug_by_ptrace
{
    void* handle = dlopen(0, RTLD_GLOBAL | RTLD_NOW);
    ptrace_ptr ptrace = dlsym(handle, "ptrace");
    ptrace(PT_DENY_ATTACH, 0, 0, 0);
    dlclose(handle);
}
-(void) anti_debug_by_syscall
{
    // SYS_ptrace 26
    syscall(26,PT_DENY_ATTACH,0,0);
}

-(void) anti_debug_by_svc
{
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
-(void) anti_debug_by_svc_syscall
{
#ifdef __arm__
    __asm__("mov r0, #26\n"
            "mov r1, #31\n"
            "mov r2, #0\n"
            "mov r3, #0\n"
            "mov r12, #16\n"
            "svc #0x80");
#endif
#ifdef __arm64__
    __asm__("mov X0, #26\n"
            "mov X1, #31\n"
            "mov X2, #0\n"
            "mov X3, #0\n"
            "mov X4, #0\n"
            "mov w16, #0\n"
            "svc #0x80");
#endif
}

-(void) check_debug_by_sysctl
{
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
    if((info.kp_proc.p_flag & P_TRACED) != 0)
    {
        exit(1);
    }
}


-(void) check_debug_by_isatty
{
  if (isatty(STDOUT_FILENO))
  {
    exit(1);
  }
}

//-(void) check_debug_by_ioctl
//{
//  NSLog(@"check_debug_by_ioctl==%d", ioctl(PT_DENY_ATTACH, 0));
//  if (ioctl(PT_DENY_ATTACH, 0) == -1)
//  {
//    exit(1);
//  }
//}

-(void) check_debug_by_ioctl
{
    NSLog(@"check_debug_by_ioctl==%d",ioctl(1, TIOCGWINSZ));
  if (!ioctl(STDOUT_FILENO, TIOCGWINSZ))
  {
    exit(1);
  }
}

@end
