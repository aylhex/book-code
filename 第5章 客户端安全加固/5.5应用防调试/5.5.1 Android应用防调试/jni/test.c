//
// Created by Ayl on 12/26/22.
//

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/ptrace.h>

#define BUFF_LEN 128
time_t start_time;
time_t end_time;



void check_debug_by_ptrace() {
    int pid = fork();
    if (pid == 0) {
        int ppid = getppid();
        if (ptrace(PTRACE_ATTACH, ppid, NULL, NULL) == 0) {
            waitpid(ppid, NULL, 0);
            ptrace(PTRACE_CONT, NULL, NULL);
        }
    }
}

void signal_handle(int sig){ 
    // 自定义信号处理函数
    exit(0);
}
void check_debug_by_signal(){
    // 设置SIGTRAP信号的处理函数为signal_handle
    long ret = (long)signal(SIGTRAP, signal_handle);
    raise(SIGTRAP);  // 主动发送SIGTRAP信号
}


void check_debug_by_tracerpid() {
    char debug_file[56]={0};
    int state = 0;
    int pid = getpid(); 
    sprintf(debug_file, "/proc/%d/status", pid);
    FILE* fp = fopen(debug_file, "r");
    char line[1024]={0};
    while(fgets(line, 1024, fp)){
        if (strncmp(line, "TracerPid", 9) == 0){
            state = atoi(&line[10]);
            if (state != 0){
                fclose(fp);
                exit(0);
            }
            break;
        }
        memset(line, 0, 1024);
    }
    fclose(fp);
}

/*
 * IDA调试的默认端口23946，对应的十六进制值5D8A
 * Frida默认会占用的两个端口27402、27403，对应的十六进制值6B0A、6B0B
 */
void check_debug_by_port()
{
    char buff[BUFF_LEN];
    char line[BUFF_LEN];
    const char* dir = "/proc/net/tcp";
    FILE *fp = fopen(dir, "r");
    while (fgets(buff, BUFF_LEN, fp) != NULL)
    {
        if (strstr(buff, "5D8A") != NULL || 
	        strstr(buff, "6B0A") != NULL || 
	        strstr(buff, "6B0B") != NULL)
        {
            fclose(fp);
            exit(0);
        }
    }
    fclose(fp);
    FILE *fd = popen("netstat -apn", "r");  
    while (fgets(line, sizeof(line), fd) != NULL)
    {

        if (strstr(line, "23946") != NULL || 
	        strstr(line, "27402") != NULL || 
	        strstr(line, "27403") != NULL)
        {
            fclose(fd);
            exit(0);
        }
    }
    pclose(fd);
}


void check_debug_by_time() {
    time(&start_time);
    /*
     * 需要监控的核心代码
     */
    time(&end_time);
    if(end_time - start_time > 10){
        exit(0);
    }
}


// void check_debug_by_isatty() {
//     if (isatty(1)) {
//         exit(1);
//     }
// }


// void check_debug_by_ioctl() {
//     if (!ioctl(1, TIOCGWINSZ)) {
//         exit(1);
//     }
// }


void check_debug()
{
    //check_debug_by_ptrace();
    check_debug_by_tracerpid();
}

int main()
{
    //check_debug();

    while (1)
    {
        //check_debug_by_tracerpid();
        /* code */
    }
    
}