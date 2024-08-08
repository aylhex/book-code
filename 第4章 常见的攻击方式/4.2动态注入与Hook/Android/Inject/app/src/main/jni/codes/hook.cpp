//
// Created by AYL on 2022/11/24.
//

#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
typedef int(*STRCMP)(const char*, const char*);
int strcmp(const char *s1, const char *s2)
{
    static void *handle = NULL;
    static STRCMP old_strcmp = NULL;
    if( !handle )
    {
        handle = dlopen("libc.so", RTLD_LAZY);
        old_strcmp = (STRCMP)dlsym(handle, "strcmp");
    }
    printf("hack function invoked. s1=<%s> s2=<%s>\n", s1, s2);
    return old_strcmp(s1, s2);
}