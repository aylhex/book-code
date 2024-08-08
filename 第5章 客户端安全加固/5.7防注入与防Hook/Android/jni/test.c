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

void anti_attach1() {

    ptrace(PTRACE_TRACEME, 0, NULL, NULL);
}


void anti_attach2() {
    int pid = fork();
    if (pid == 0) {
        int ppid = getppid();
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        ptrace(PTRACE_ATTACH, ppid, 0, 0);
        waitpid(ppid, NULL, 0);
        ptrace(PTRACE_CONT, ppid, 0, 0);
        
    } 
}

int check_block_list(char* libpath)
{
    // 黑单示例
    char* block_list[2] = {"/data/local/tmp", "package"};
    for(int i = 0; i < 2; i++)
    {
        if (strnstr(libpath ,block_list[i], strlen(block_list[i])) == NULL)
        {
            return 1;
        }
    }

    return -1;
}


/*
hook注入检测
返回值为0 说明没有异常库信息
正常返回异常库的个数。
根据type的值不同 检测hook框架和注入
 */ 
void check_inject()
{
    char pFilePath[32];
    char pLibInfo[256];
    char *pLibPath = NULL;
    char *savePtr = NULL;
    int pid = getpid();
    sprintf(pFilePath, "/proc/%d/maps", pid);
    FILE *fp = fopen(pFilePath, "r");
    while (fgets(pLibInfo, sizeof(pLibInfo), fp) != NULL) 
    {
        strtok_r(pLibInfo, " \t", &savePtr); // 地址信息
        strtok_r(NULL, " \t", &savePtr); // 权限信息
        strtok_r(NULL, " \t", &savePtr); // 偏移信息
        strtok_r(NULL, " \t", &savePtr); // 设备信息
        strtok_r(NULL, " \t", &savePtr); // 节点信息
        pLibPath = strtok_r(NULL, " \t", &savePtr); // 路径信息
        if (pLibPath != NULL) // 判断是否有路径信息
        {
            if (check_block_list(pLibPath) == -1)
            {
                exit(0);
            } 
        }
        memset(pLibInfo, 0, 256);
    }
    fclose(fp);
}

void anti_attach()
{
    anti_attach1();
    anti_attach2();
}

int main()
{
    anti_attach();
}