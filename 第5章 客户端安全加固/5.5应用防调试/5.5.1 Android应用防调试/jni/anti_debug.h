//
// Created by Ayl on 12/26/22.
//

#ifndef ANTI_DEBUG_H
#define ANTI_DEBUG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif 
	void check_debug();
    void check_debug_by_ptrace();
    void check_debug_by_tracerpid();
    void check_debug_by_port();
    void check_debug_by_time();

#ifdef __cplusplus
}
#endif

#endif //ANTI_DEBUG_H

