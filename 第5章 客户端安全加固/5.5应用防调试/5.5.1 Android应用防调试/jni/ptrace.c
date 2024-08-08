#include <sys/ptrace.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/syscall.h>

int main(int argc, char *argv[])
{
    int status;
    if (argc < 2) 
    {
        printf("Usage: %s <pid>\n", argv[0]);
        return 1;
    }
    pid_t pid = atoi(argv[1]);
    if (ptrace(PTRACE_ATTACH, pid, 0, 0) < 0) 
    {
        printf("Cannt attch target pid ptrace %d \n", pid);
        return 1;
    }
    ptrace(PTRACE_CONT, pid, NULL, NULL);
    printf("The process with pid %d is already attached！！！\n", pid);

    while (1) {
        waitpid(pid, &status, 0);
        if (WIFSTOPPED(status)) 
        {
            // The process has stopped; handle the stop event
            // ...  
            // Continue execution of the process
            ptrace(PTRACE_CONT, pid, NULL, NULL);
        } else if (WIFEXITED(status)) 
        {
        // The process has exited; break out of the loop
            break;
        }
    }


    //wait(NULL);
    printf("Press any key to terminate attach\n");
    getchar();
    ptrace(PTRACE_DETACH, pid, 0, 0);
    return 0;
}